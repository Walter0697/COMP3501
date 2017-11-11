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
	}

	/* Destructor */
	DragonFly::~DragonFly() {}

	
	/* Update */
	void DragonFly::update()
	{
		state = rand() % 2;

		if (state == 0) { //Idle
			body->Translate(glm::vec3(1, 0, 0));
		}
		else if (state == 1) { //Move to player
			body->Translate(glm::vec3(-1, 0, 0));
		}
		else if (state == 2) { //Attack

		}
		else if (state == 3) { //Patrol

		}
		else { std::cout << "Invalid state in Dragonfly" << std::endl; }
	}

	/* Collision */
	bool DragonFly::collision(SceneNode* object)
	{
		return false;
	}
}