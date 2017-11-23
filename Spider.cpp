#include "Spider.h"

namespace game
{
	Spider::Spider(SceneNode* spiderBody, SceneNode* spiderLeftLeg, SceneNode* spiderRightLeg)
	{
		targetPos = glm::vec3(0, 0, 0);				// Store target position
		targetOrientation = glm::quat(0, 0, 0, 0);	// Store target orientation
		lastUpdate = -1;							// Last update time
		updateTime = 0.5;							// Update time
		state = 0;									// Machine state

		body = spiderBody;							// Body of the spider node
		leftLeg = spiderLeftLeg;					// Left leg of the spider node
		rightLeg = spiderRightLeg;					// Right leg of the spider node
			
		speed = 1.0;								// Speed of movement the spider
		fireRate = 0;								// FireRate of the shooting webs
		maxFireRate = 50;							// Maximum fire rate of shooting
		maxHealth = 30;								// Maximum health
		health = maxHealth;							// Health

		firing = false;								//Controls if the enemy is shooting
		shotTimer = -1.f;
		fireRate = 1.0f;

		timer = 5;
		legMovement = true;
		isMoving = false;
	}

	Spider::~Spider() {}

	/* Update */
	void Spider::update()
	{
		if (isMoving)
			if (legMovement)
			{
				timer--;
				leftLeg->Translate(glm::vec3(0, 0, 0.005));
				rightLeg->Translate(glm::vec3(0, 0, -0.005));
				if (timer <= 0) { legMovement = false; }
			}
			else
			{
				timer++;
				leftLeg->Translate(glm::vec3(0, 0, -0.005));
				rightLeg->Translate(glm::vec3(0, 0, 0.005));
				if (timer >= 10) { legMovement = true; }
			}


		time_t t = time(0);
		if (lastUpdate == -1 || t - lastUpdate > updateTime) {
			state = int(rand() % 3);
			lastUpdate = t;
			//std::cout << "************SWITCH TO " << state << std::endl;
		}
		else {
			//std::cout << "no switch yet, t - lastUpdate = " << t << ",  " <<  lastUpdate << std::endl;
		}

		if (state == 0) { //Idle
			body->Translate(glm::vec3(0, 0, 0));
			isMoving = false;
		}
		else if (state == 1) { //Move to player
			glEnable(GL_NORMALIZE);
			body->Translate(glm::vec3(((targetPos.x - body->GetPosition().x) * 0.01), ((targetPos.y - body->GetPosition().y) * 0.01), ((targetPos.z - body->GetPosition().z) * 0.01)));
			isMoving = true;
		}
		else if (state == 2) { //Attack
			if (glfwGetTime() - this->shotTimer >= this->fireRate) {
				this->firing = true;
				this->shotTimer = glfwGetTime();
			}
			else {
				state = 0;
			}
			isMoving = true;
		}
		else if (state == 3) { //Patrol

		}
		else {
			std::cout << "Invalid state in Spider" << std::endl;
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

	bool Spider::collision(SceneNode* object)
	{
		return false;
	}
}