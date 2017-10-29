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

namespace game 
{
    class Fly : public SceneNode
	{
	public:
		Fly(std::string , const Resource* , const Resource* , const Resource*);
		~Fly();

		float speed;
		std::vector<Rocket*> rockets; //store the rockets
		int fireRate;
		int maxFireRate;

		virtual void Update();	//update fly and all things that are related to it
	private:
	protected:
    }; 
} // namespace game
#endif //FLY_H
