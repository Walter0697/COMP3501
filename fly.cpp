#include "fly.h"

namespace game 
{
	/* Constructor */
	Fly::Fly(SceneNode* flyBody, SceneNode* flyWings, SceneNode* flyLegs)
	{
		speed = 0.5;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		timer = 10;						// Timer for wing movement
		upWingMovement = true;			// Checking for upwards or downwards fly rotation
		maxHealth = 100;				// Maximum health
		health = maxHealth;				// Health of fly
		boundingRadius = 0.7;			// Radius of bounds
		body = flyBody;					// Body of fly
		wings = flyWings;				// Wings of the fly
		legs = flyLegs;					// Legs of fly
	}

	/* Destructor */
	Fly::~Fly(){}

	/* Update */
	void Fly::update()
	{
		//prevPosition = body->getAbsolutePosition();
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
				rockets[i]->node->del = true;
				rockets.erase(rockets.begin() + i); 
			}
			else { rockets[i]->update(); }
		}
	}

	/* Collision */
	bool Fly::collision(SceneNode * object, float boundRad)
	{
		glm::vec3 difference = body->getAbsolutePosition() - object->getAbsolutePosition();
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad);
	}
} // namespace game
