#ifndef WALL_H
#define WALL_H

#include "scene_node.h"
#include "Collidable.h"

namespace game
{
	class Wall : public Collidable
	{
	public:
		Wall(SceneNode*, glm::vec3, float, float);
		~Wall();

		SceneNode* node;
		glm::vec3 normal;
		float length;
		float width;
		bool collision(SceneNode*, float, float);
		bool collision(SceneNode*, float, float, glm::vec3*);
	};
}
#endif // WALL_H