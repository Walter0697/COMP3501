#include "fly.h"

namespace game 
{
	Fly::Fly(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture)
	{
		speed = 2.0;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		visible = true;

		timer = 5;
		up = true;
		
		maxHealth = 100;
		health = maxHealth;
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
		if (up)
		{
			timer--;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 180, glm::vec3(5, 0, 0)));
			wing->Rotate(rotation);
			if (timer <= 0)
				up = false;
		}
		else
		{
			timer++;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 180, glm::vec3(-5, 0, 0)));
			wing->Rotate(rotation);
			if (timer >= 5)
				up = true;
		}
	}

	bool Fly::collision(SceneNode * object)
	{
		return false;
	}
} // namespace game;
