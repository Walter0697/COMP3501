#ifndef ENEMY_H
#define ENEMY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <ctime>
#include "Character.h"
#include "Rocket.h"

// CREATE A GENERAL CHARACTER WHICH WILL BE INHERITED BY ALL CHARACTER TYPES 
namespace game
{
	class Enemy : public Character
	{
	public:
		virtual void updateTarget(glm::vec3 targPos) { targetPos = targPos; }

		virtual	void updateTargetOrientation(glm::quat orient)
		{
			targetOrientation = orient;
			glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			targetOrientation = rotation * targetOrientation;
		}
		virtual bool getFiring() { return firing; }
		virtual void fire(Rocket* r) {
			if (firing) {
				firing = false;
				rockets.push_back(r);
			}
		}

	private:
	protected:
		glm::vec3 targetPos;
		glm::quat targetOrientation;
		time_t lastUpdate;
		time_t updateTime;
		int state;
		std::vector<Rocket*> rockets; //store the rockets
		bool firing;
		float shotTimer;
		float fireRate;


	};
}
#endif // ENEMY_H