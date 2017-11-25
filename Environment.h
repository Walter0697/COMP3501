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

		bool collision(SceneNode* , float);

	private:
	protected:
	};
}

#endif // ENVIRONMENT_H