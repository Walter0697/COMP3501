#include "Human.h"

namespace game
{
	/* Constructor */
	Human::Human(SceneNode* humanBody, SceneNode* humanLeftHand, SceneNode* humanRightHand, SceneNode* humanLeftLeg, SceneNode* humanRightLeg)
	{
		
		targetPos = glm::vec3(0, 0, 0);					// Store target position
		targetOrientation = glm::quat(0, 0, 0, 0);		// Store target orientation
		lastUpdate = -1;								// Last update time
		updateTime = 0.5;								// Update time
		state = 0;										// State in state machine 

		speed = 0.3;									// Human speed of movement
		fireRate = 0;									// Human fireRate 
		maxFireRate = 60;								// Human maxFireRate
		maxHealth = 40;									// Max health
		health = maxHealth;								// Health

		body = humanBody;								// Body of the human node
		leftHand = humanLeftHand;						// Left hand of the human node
		rightHand = humanRightHand;						// Right hand of the human node
		leftLeg = humanLeftLeg;							// Left leg of the human node
		rightLeg = humanRightLeg;						// Right leg of the human node

		firing = false;									//Controls if the enemy is shooting
		shotTimer = -1.f;								

		boundingRadius = 4.0;
		onFloor = false;
		gravity = -0.2f;
	}

	/* Destructor */
	Human::~Human() {}

	/* Update */
	void Human::update() 
	{
		// find forward direction of enemy
		prevDirection = glm::normalize(direction);
		direction = glm::normalize(targetPos - body->getAbsolutePosition());

		if (!onFloor) { body->Translate(glm::vec3(0, gravity, 0)); }

		body->SetOrientation(targetOrientation);
		
		time_t t = time(0);
		if (lastUpdate == -1 || t - lastUpdate > updateTime) 
		{
			state = int(rand() % 3);
			lastUpdate = t;
			//std::cout << "************SWITCH TO " << state << std::endl;
		}

		if (state == 0) 
		{ 
			//Idle
			body->Translate(glm::vec3(0, 0, 0));
		}
		else if (state == 1) 
		{
			//Move to player
			body->SetOrientation(targetOrientation);

			glEnable(GL_NORMALIZE);
			//body->Translate(glm::vec3(((targetPos.x - body->getAbsolutePosition().x) * 0.01),
				//					  ((targetPos.y - body->getAbsolutePosition().y) * 0.01),
					//				  ((targetPos.z - body->getAbsolutePosition().z) * 0.01)));
		}
		else if (state == 2) 
		{ 
			//Attack
			if (glfwGetTime() - this->shotTimer >= this->fireRate) 
			{
				this->firing = true;
				this->shotTimer = glfwGetTime();
			}
			else { state = 0; }
		}
		else if (state == 3) {} //Patrol
		else { std::cout << "Invalid state in Human" << std::endl; }

		// Check rockets 
		for (int i = 0; i < projectiles.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (projectiles[i]->timer <= 0) { projectiles .erase(projectiles.begin() + i); }
			else { projectiles[i]->update(); }
		}
	}

	/* Collision */
	bool Human::collision(SceneNode * object, float boundRad)
	{
		return false;
	}
}
