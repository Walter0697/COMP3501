#ifndef WALL_H
#define WALL_H

#include "scene_node.h"
#include "Collidable.h"

namespace game
{
	class Wall : public Collidable
	{
	public:
		Wall(SceneNode*, glm::vec3, float, float, float, float);
		~Wall();

		SceneNode* node;
		glm::vec3 normal;
		float minX;
		float minY;
		float maxX;
		float maxY;
		bool collision(SceneNode*, float);
		bool collision(SceneNode*, float, glm::vec3*);
	};
}
#endif // WALL_H