#include "Human.h"

namespace game
{
	Human::Human(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) :  SceneNode(name, geometry, material, texture)
	{
		targetPos = glm::vec3(0, 0, 0);
		orientation = glm::quat(0, 0, 0, 0);
		lastUpdate = -1;
		updateTime = 0.5;
		state = 0;

		speed = 3.0;		// Human speed of movement
		fireRate = 0;		// Human fireRate 
		maxFireRate = 30;	// Human maxFireRate
		maxHealth = 40;
		health = maxHealth;
	}

	Human::~Human() {}

	void Human::Update() {
		time_t t = time(0);
		if (lastUpdate == -1 || t - lastUpdate > updateTime) {
			state = int(rand() % 2);
			lastUpdate = t;
			//std::cout << "************SWITCH TO " << state << std::endl;
		}
		else {
			//std::cout << "no switch yet, t - lastUpdate = " << t << ",  " <<  lastUpdate << std::endl;
		}


		if (state == 0) { //Idle
			Translate(glm::vec3(0, 0, 0));
		}
		else if (state == 1) { //Move to player
			SetOrientation(orientation);
			glEnable(GL_NORMALIZE);
			Translate(glm::vec3(((targetPos.x - GetPosition().x) * 0.01), 
								((targetPos.y - GetPosition().y) * 0.01), 
								((targetPos.z - GetPosition().z) * 0.01)));
		}
		else if (state == 2) { //Attack

		}
		else if (state == 3) { //Patrol

		}
		else {
			std::cout << "Invalid state in Human" << std::endl;
		}
	}

	void Human::UpdateOrientation(glm::quat orient) {
		orientation = orient;
		glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0,1,0));
		orientation = rotation * orientation;
	}

	void Human::UpdateTarget(glm::vec3 targPos) {
		targetPos = targPos;
	}

	bool Human::collision(SceneNode * object)
	{
		return false;
	}
}
