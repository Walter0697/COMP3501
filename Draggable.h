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
		virtual bool getDrag() { return drag; };
		virtual void setDrag(bool b) { drag = b; };
		virtual void setDragger(glm::vec3 s) { dragger = s; }

		bool drag;
		glm::vec3 dragger;

	};
}
#endif // DRAGGABLE_H