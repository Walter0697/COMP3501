#ifndef WEB_H
#define WEB_H

#include <string>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

#include "scene_node.h"
#include "Collidable.h"

// Rockets 
namespace game
{
	class Web : public Collidable
	{
	public:
		Web(SceneNode*, glm::vec3);
		~Web();

		glm::vec3 direction;	// direction of the rocket
		float speed;			// speed of the rocket
		int timer;				// time after which rocket gets deleted 
		SceneNode* webNode;	// rocket node

		virtual void update();	// update the position of the rocket
		//bool collision(SceneNode*, Camera*);	// collision detection between rocket and other collidables
		bool collision(SceneNode*, float);

	private:
	protected:
	};
}
#endif // WEB_H