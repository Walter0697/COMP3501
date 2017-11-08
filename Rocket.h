#ifndef ROCKET_H
#define ROCKET_H

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "scene_node.h"
#include "camera.h"
#include "Collidable.h"

namespace game
{
	class Rocket : public SceneNode , public Collidable
	{
	public:
		Rocket(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture , glm::vec3);
		~Rocket();

		glm::vec3 direction;	// direction of the rocket
		float speed;			// speed of the rocket
		int timer;				// time after which rocket gets deleted 

		virtual void Update();	// update the position of the rocket
		bool collision(SceneNode* , Camera*);	// collision detection between rocket and other collidables
		bool collision(SceneNode*);
	private:

	protected:
	};
}
#endif // !ROCKET_H