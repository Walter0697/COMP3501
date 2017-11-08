#include "fly.h"

namespace game 
{
	Fly::Fly(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture)
	{
		speed = 2.0;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		visible = true;
		timer = 10;						// Timer for wing movement
		upWingMovement = false;			// Checking for upwards or downwards fly rotation
		wing = NULL;					// Wings of the fly
		maxHealth = 100;				// Maximum health
		health = maxHealth;				// Health of fly
	}

	Fly::~Fly(){}

	void Fly::Update()
	{
		fireRate--;		

		for (int i = 0; i < rockets.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (rockets[i]->timer <= 0)
			{
				rockets[i]->visible = false;
				rockets.erase(rockets.begin() + i);
			}
		}
		
		//wing animation
		if (upWingMovement)
		{
			timer--;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 180, glm::vec3(1, 0, 0)));
			wing->Rotate(rotation);
			if (timer <= 0)
				upWingMovement = false;
		}
		else
		{
			timer++;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 180, glm::vec3(-1, 0, 0)));
			wing->Rotate(rotation);
			if (timer >= 10) { upWingMovement = true; }
		}
	}

	bool Fly::collision(SceneNode * object)
	{
		return false;
	}
} // namespace game;
