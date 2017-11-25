#ifndef FLY_H
#define FLY_H

#include <string>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <stdexcept>
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <time.h>

#include "resource.h"
#include "camera.h"
#include "scene_node.h"
#include "Rocket.h"
#include "Character.h"

// FLIES ARE COLLIDABLES AND CHARACTERS
namespace game 
{
    class Fly : public Character
	{
	public:
		Fly(SceneNode*,SceneNode*,SceneNode*);
		~Fly();

		std::vector<Rocket*> rockets; //store the rockets

		SceneNode* body;
		SceneNode* wings;
		SceneNode* legs;

		int timer;
		bool upWingMovement;
		bool moving;

		void update();	//update fly and all things that are related to it
		bool collision(SceneNode*, float);

	private:
	protected:
    }; 
} // namespace game
#endif //FLY_H
