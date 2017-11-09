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

// CREATE A GENERAL CHARACTER WHICH WILL BE INHERITED BY ALL CHARACTER TYPES 
namespace game
{
	class Enemy : public Character
	{
	public:
		virtual void UpdateTarget(glm::vec3 targPos) = 0;
		virtual void UpdateOrientation(glm::quat orient) = 0;

	private:
	protected:
		glm::vec3 targetPos;
		glm::quat orientation;
		time_t lastUpdate;
		time_t updateTime;
		int state;

	};
}
#endif // ENEMY_H