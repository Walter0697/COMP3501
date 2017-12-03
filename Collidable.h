#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "scene_node.h"

// INTERFACE FOR ALL COLLIDABLES OBJECTS
namespace game
{
	class Collidable
	{
	public:
		float offset;
		float boundingRadius;
		virtual bool collision(SceneNode*, float, float) = 0;
	};
}
#endif // COLLIDABLE_H