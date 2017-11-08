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
#include "Collidable.h"

// FLIES ARE COLLIDABLES SCENENODES AND CHARACTERS
namespace game 
{
    class Fly : public SceneNode , public Character , public Collidable
	{
	public:
		Fly(std::string = "" , const Resource* = 0, const Resource* = 0, const Resource* = 0);
		~Fly();

		std::vector<Rocket*> rockets; //store the rockets

		virtual void Update();	//update fly and all things that are related to it
		bool collision(SceneNode*);
	private:
	protected:
    }; 
} // namespace game
#endif //FLY_H
