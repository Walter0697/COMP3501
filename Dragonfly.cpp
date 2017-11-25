#include "DragonFly.h"

namespace game
{
	/* Constructor */
	DragonFly::DragonFly(SceneNode* dragonFlyBody, SceneNode* dragonFlyLeftWing, SceneNode* dragonFlyRightWing, SceneNode* dragonFlyLegs)
	{
		body = dragonFlyBody;							// Body of the dragonfly node
		leftWing = dragonFlyLeftWing;					// Left wing of the dragonfly node
		rightWing = dragonFlyRightWing;					// Right wing of the dragonfly node
		legs = dragonFlyLegs;							// Legs of the dragonfly node

		targetPos = glm::vec3(0, 0, 0);					// Store target position
		targetOrientation = glm::quat(0, 0, 0, 0);		// Store target orientation
		lastUpdate = -1;								// Previous update time
		updateTime = 0.5;								// Update time
		state = 0;										// State in game Machine 

		speed = 0.2;									// Speed of dragon fly
		fireRate = 0;									// Fire rate of dragonfly shots 
		maxFireRate = 90;								// maxFireRate of dragonfly
		maxHealth = 20;									// Max health 
		health = maxHealth;								// Health

		firing = false;									//Controls if the enemy is shooting
		shotTimer = -1.f;
		fireRate = 20.0f;

		timer = 12;
		upWingMovement = true;

		boundingRadius = 0.7;
	}

	/* Destructor */
	DragonFly::~DragonFly() {}

	
	/* Update */
	void DragonFly::update()
	{
		//wing animation
		if (upWingMovement)
		{
			timer--;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(0, 0, -1)));
			leftWing->Rotate(rotation);
			rotation = glm::quat(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(0, 0, 1)));
			rightWing->Rotate(rotation);
			if (timer <= 0) { upWingMovement = false; }
		}
		else
		{
			timer++;
			glm::quat rotation(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(0, 0, 1)));
			leftWing->Rotate(rotation);
			rotation = glm::quat(glm::angleAxis(glm::pi<float>() / 120, glm::vec3(0, 0, -1)));
			rightWing->Rotate(rotation);
			if (timer >= 12) { upWingMovement = true; }
		}

		state = rand() % 3;

		if (state == 0) { //Idle
			body->Translate(glm::vec3(0, 0, 0));
		}
		else if (state == 1) {
			//Move to player

			body->SetOrientation(targetOrientation);

			glEnable(GL_NORMALIZE);
			body->Translate(glm::vec3(((targetPos.x - body->getAbsolutePosition().x) * 0.01),
				((targetPos.y - body->getAbsolutePosition().y) * 0.01),
				((targetPos.z - body->getAbsolutePosition().z) * 0.01)));
		}
		else if (state == 2) { //Attack
			if (glfwGetTime() - this->shotTimer >= this->fireRate) {
				this->firing = true;
				this->shotTimer = glfwGetTime();
			}
			else {
				state = 0;
			}
		}
		else if (state == 3) { //Patrol

		}
		else {
			std::cout << "Invalid state in Dragonfly" << std::endl;
		}

		// Check rockets 
		for (int i = 0; i < rockets.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (rockets[i]->timer <= 0) { rockets.erase(rockets.begin() + i); }
			else { rockets[i]->Update(); }
		}
	}

	/* Collision */
	bool DragonFly::collision(SceneNode* object, float boundRad)
	{
		glm::vec3 difference = body->getAbsolutePosition() - object->getAbsolutePosition();
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad);
	}
}