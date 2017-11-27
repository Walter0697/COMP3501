#include "wall.h"

namespace game
{
	Wall::Wall(SceneNode* wallNode, glm::vec3 myNormal , float Xmin, float Ymin, float Xmax, float Ymax)
	{
		node = wallNode;
		normal = glm::normalize(myNormal);
		minX = Xmin;
		minY = Ymin;
		maxX = Xmax;
		maxY = Ymax;
	}

	Wall::~Wall() {}

	bool Wall::collision(SceneNode* obj, float boundRad) { return false; }

	//plane sphere collision (still need to check min and max x and y values)
	//check if within plane
	bool Wall::collision(SceneNode* obj, float boundRad, glm::vec3* norm)
	{
		*norm = normal;
		glm::vec3 wallObjVec = node->getAbsolutePosition() - obj->getAbsolutePosition();
		float prod = abs(glm::dot(wallObjVec, normal));
		return prod <= boundRad;
	}
}