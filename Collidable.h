#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "scene_node.h"

// INTERFACE FOR ALL COLLIDABLES OBJECTS
namespace game
{
	class Collidable
	{
	public:
		virtual bool collision(SceneNode*) = 0;
	};
}
#endif // COLLIDABLE_H