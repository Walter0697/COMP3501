#ifndef DRAGGABLE_H
#define DRAGGABLE_H

#include "scene_node.h"

// INTERFACE FOR ALL DRAGGABLE OBJECTS
namespace game
{
	class Draggable
	{
		virtual bool dragging(SceneNode*) = 0;

		bool drag;
	};
}
#endif // DRAGGABLE_H