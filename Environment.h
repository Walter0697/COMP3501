#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "scene_node.h"
#include "Collidable.h"

// SPIDERS ARE CHARACTERS COLLIDABLES 
namespace game
{
	class Environment : Collidable
	{
	public:
		Environment(SceneNode*);
		~Environment();

		SceneNode* floor;

		glm::vec3 normals;
		bool collision(SceneNode* , float);
		bool collision(SceneNode*, float, glm::vec3);

	private:
	protected:
	};
}

#endif // ENVIRONMENT_H