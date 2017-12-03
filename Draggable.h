#ifndef DRAGGABLE_H
#define DRAGGABLE_H

#include "scene_node.h"
#include "Collidable.h"
#include <glm/glm.hpp>

// INTERFACE FOR ALL DRAGGABLE OBJECTS
namespace game
{
	class Draggable : public Collidable
	{
	public:
		bool beingDragged;
		bool onFloor;
		float gravity;
	};
}
#endif // DRAGGABLE_H