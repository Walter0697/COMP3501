#include "DragonFly.h"

namespace game
{
	/* Constructor */
	DragonFly::DragonFly(std::string name, const Resource* geom, const Resource* mat, const Resource* texture) : SceneNode(name, geom, mat, texture)
	{
		targetPos = glm::vec3(0, 0, 0);
		orientation = glm::quat(0, 0, 0, 0);
		lastUpdate = -1;
		updateTime = 0.5;
		state = 0;

		speed = 3.0;
		fireRate = 0;
		maxFireRate = 90;
		maxHealth = 20;
		health = maxHealth;
	}

	/* Destructor */
	DragonFly::~DragonFly() {}

	/* Update */
	void DragonFly::Update()
	{
		state = rand() % 2;

		if (state == 0) { //Idle
			Translate(glm::vec3(1, 0, 0));
		}
		else if (state == 1) { //Move to player
			Translate(glm::vec3(-1, 0, 0));
		}
		else if (state == 2) { //Attack

		}
		else if (state == 3) { //Patrol

		}
		else {
			std::cout << "Invalid state in Dragonfly" << std::endl;
		}
	}

	void DragonFly::UpdateOrientation(glm::quat orient) {
		orientation = orient;
		glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
		orientation = rotation * orientation;
	}

	void DragonFly::UpdateTarget(glm::vec3 targPos) {
		targetPos = targPos;
	}


	/* Collision */
	bool DragonFly::collision(SceneNode* object)
	{
		return false;
	}
}