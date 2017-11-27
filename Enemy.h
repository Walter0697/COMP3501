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
#include "Projectile.h"

// CREATE A GENERAL CHARACTER WHICH WILL BE INHERITED BY ALL CHARACTER TYPES 
namespace game
{
	class Enemy : public Character
	{
	public:
		
		glm::vec3 getDirection() { return direction; }

		virtual void updateTarget(glm::vec3 targPos) 
		{
			targetPos = targPos; 
		}

		virtual	void updateTargetOrientation(glm::quat orient)
		{
			targetOrientation = orient;
			glm::quat rotation = glm::angleAxis(glm::pi<float>(), glm::vec3(0, 1, 0));
			targetOrientation = rotation * targetOrientation;
		}

		virtual bool getFiring() { return firing; }

		virtual void fire(Projectile* p) {
			if (firing) {
				firing = false;
				projectiles.push_back(p);
			}
		}


	private:
	protected:
		glm::vec3 direction;
		glm::vec3 prevDirection;
		glm::vec3 targetPos;
		glm::quat targetOrientation;
		time_t lastUpdate;
		time_t updateTime;
		int state;
		std::vector<Projectile*> projectiles;   //store the projectiles
		bool firing;
		float shotTimer;
		float fireRate;
	};
}
#endif // ENEMY_H