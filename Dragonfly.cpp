#include "DragonFly.h"

namespace game
{
	/* Constructor */
	DragonFly::DragonFly(SceneNode* dragonFlyBody, SceneNode* dragonFlyLeftWing, SceneNode* dragonFlyRightWing, SceneNode* dragonFlyLegs)
	{
		targetPos = glm::vec3(0, 0, 0);					// Store target position
		targetOrientation = glm::quat(0, 0, 0, 0);		// Store target orientation
		lastUpdate = -1;								// Previous update time
		updateTime = 0.5;								// Update time
		state = 0;										// State in game Machine 
		speed = 1;									// Speed of dragon fly
		fireRate = 0;									// Fire rate of dragonfly shots 
		maxFireRate = 150;								// maxFireRate of dragonfly
		maxHealth = 20;									// Max health 
		health = maxHealth;								// Health
		firing = false;									//Controls if the enemy is shooting
		shotTimer = -1.f;
		timer = 12;										// Timer for wing animation
		upWingMovement = true;							// Check for wing movement
		boundingRadius = 0.5;							// radius bound
		offset = 0.3;									// center offset
		direction = glm::vec3(0, 0, 0);					// Direction of movement
		prevDirection = glm::vec3(0, 0, 0);				// Previous direction for setting up orientation
		body = dragonFlyBody;							// Body of the dragonfly node
		leftWing = dragonFlyLeftWing;					// Left wing of the dragonfly node
		rightWing = dragonFlyRightWing;					// Right wing of the dragonfly node
		legs = dragonFlyLegs;							// Legs of the dragonfly node
	}

	/* Destructor */
	DragonFly::~DragonFly() {}

	
	/* Update */
	void DragonFly::update()
	{
		fireRate--;
		// Set forward direction and store previous one
		prevDirection = direction;
		direction = glm::normalize(targetPos - body->getAbsolutePosition());

		glm::quat rotation(prevDirection,direction);
		body->Rotate(rotation);

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

		//setup state machine
		state = rand() % 3;

		if (state == 0) {}  //Idle
		else if (state == 1) 
		{
			//Move to player
			body->Translate(direction * speed);
		}
		else if (state == 2) 
		{ 
			//Attack
			if (fireRate <= 0)
			{
				this->firing = true;
				fireRate = maxFireRate;
			}
			else { state = 0; }
		}
		else if (state == 3) { } //Patrol
		else { std::cout << "Invalid state in Dragonfly" << std::endl; }

		// Check rockets when timer is 0 delete the rocket
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (projectiles[i]->timer <= 0) { projectiles.erase(projectiles.begin() + i); }
			else { projectiles[i]->update(); }
		}
	}

	/* Collision */
	bool DragonFly::collision(SceneNode* object, float off, float boundRad)
	{
		//find real center of the object
		glm::vec3 objUpVec = glm::normalize(object->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 objRealCenter = object->getAbsolutePosition() + objUpVec * off;

		//find my real center
		glm::vec3 myUpVec = glm::normalize(body->getAbsoluteOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 myRealCenter = body->getAbsolutePosition() + myUpVec * offset;

		//find difference in positions
		glm::vec3 difference = myRealCenter - objRealCenter;

		//compare distances 
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad + boundingRadius);
	}
}