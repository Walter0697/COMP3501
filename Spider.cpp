#include "Spider.h"

namespace game
{
	Spider::Spider(std::string name, const Resource* geom, const Resource* mat, const Resource* texture) : SceneNode(name, geom, mat, texture)
	{
		targetPos = glm::vec3(0, 0, 0);
		orientation = glm::quat(0, 0, 0, 0);
		lastUpdate = -1;
		updateTime = 0.5;
		state = 0;

		speed = 1.0;
		fireRate = 0;
		maxFireRate = 50;
		maxHealth = 30;
		health = maxHealth;
	}

	Spider::~Spider() {}

	void Spider::Update()
	{
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
			glEnable(GL_NORMALIZE);
			Translate(glm::vec3(((targetPos.x - GetPosition().x) * 0.01), ((targetPos.y - GetPosition().y) * 0.01), ((targetPos.z - GetPosition().z) * 0.01)));
		}
		else if (state == 2) { //Attack

		}
		else if (state == 3) { //Patrol

		}
		else {
			std::cout << "Invalid state in Human" << std::endl;
		}
	}


	void Spider::UpdateOrientation(glm::quat orient) {
		orientation = orient;
		glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
		orientation = rotation * orientation;
	}

	void Spider::UpdateTarget(glm::vec3 targPos) {
		targetPos = targPos;
	}


	bool Spider::collision(SceneNode* object)
	{
		return false;
	}
}