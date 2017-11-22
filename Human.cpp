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
		maxFireRate = 30;								// Human maxFireRate
		maxHealth = 40;									// Max health
		health = maxHealth;								// Health

		body = humanBody;								// Body of the human node
		leftHand = humanLeftHand;						// Left hand of the human node
		rightHand = humanRightHand;						// Right hand of the human node
		leftLeg = humanLeftLeg;							// Left leg of the human node
		rightLeg = humanRightLeg;						// Right leg of the human node

		firing = false;									//Controls if the enemy is shooting
		shotTimer = -1.f;
		fireRate = 1.0f;
	}

	/* Destructor */
	Human::~Human() {}

	/* Update */
	void Human::update() 
	{
		time_t t = time(0);
		if (lastUpdate == -1 || t - lastUpdate > updateTime) {
			state = int(rand() % 3);
			lastUpdate = t;
			//std::cout << "************SWITCH TO " << state << std::endl;
		}
		else 
		{
			//std::cout << "no switch yet, t - lastUpdate = " << t << ",  " <<  lastUpdate << std::endl;
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
			body->Translate(glm::vec3(((targetPos.x - body->getAbsolutePosition().x) * 0.01),
									  ((targetPos.y - body->getAbsolutePosition().y) * 0.01),
									  ((targetPos.z - body->getAbsolutePosition().z) * 0.01)));
		}
		else if (state == 2) 
		{ 
			//Attack
			if (glfwGetTime() - this->shotTimer >= this->fireRate) {
				this->firing = true;
				this->shotTimer = glfwGetTime();
			}
			else {
				state = 0;
			}
		}
		else if (state == 3) 
		{ 
			//Patrol
		}
		else { 
			std::cout << "Invalid state in Human" << std::endl; 
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

	/* Collision */
	bool Human::collision(SceneNode * object)
	{
		return false;
	}
}
