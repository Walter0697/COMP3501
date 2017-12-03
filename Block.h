#ifndef BLOCK_H
#define BLOCK_H

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
//#include "Rocket.h"
#include "Draggable.h"

// FLIES ARE COLLIDABLES AND CHARACTERS
namespace game
{
	class Block : public Draggable
	{
	public:
		Block(SceneNode*);
		~Block();

		SceneNode* object;

		void update();		//update block and all things that are related to it
		bool collision(SceneNode*, float , float);

	private:
	protected:
	};
} // namespace game
#endif //BLOCK_H
