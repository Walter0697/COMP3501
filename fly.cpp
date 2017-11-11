#include "fly.h"

namespace game 
{
	Fly::Fly(SceneNode* flyBody, SceneNode* flyWings, SceneNode* flyLegs)
	{
		body = flyBody;					// Body of fly
		wings = flyWings;				// Wings of the fly
		legs = flyLegs;					// Legs of fly

		speed = 2.0;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		timer = 10;						// Timer for wing movement
		upWingMovement = true;			// Checking for upwards or downwards fly rotation
		maxHealth = 100;				// Maximum health
		health = maxHealth;				// Health of fly
	}

	Fly::~Fly(){}

	void Fly::update()
	{
		fireRate--;		
		
		
		// wing animation
		if (upWingMovement)
		{
			timer--;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(1, 0, 0)));
			wings->Rotate(rotation);
			if (timer <= 0) { upWingMovement = false; }
		}
		else
		{
			timer++;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(-1, 0, 0)));
			wings->Rotate(rotation);
			if (timer >= 10) { upWingMovement = true; }
		}
		
		// Check rockets 
		for (int i = 0; i < rockets.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (rockets[i]->timer <= 0) 
			{ 
				rockets[i]->rocketNode->del = true;
				rockets.erase(rockets.begin() + i); 
			}
			else { rockets[i]->Update(); }
		}
	}

	bool Fly::collision(SceneNode * object)
	{
		return false;
	}
} // namespace game;
