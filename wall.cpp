#include "wall.h"

namespace game
{
	Wall::Wall(SceneNode* wallNode, glm::vec3 myNormal , float length , float width)
	{
		node = wallNode;
		normal = glm::normalize(myNormal);
		this->length = length;
		this->width = width;
	}

	Wall::~Wall() {}

	bool Wall::collision(SceneNode* obj, float boundRad) { return false; }

	//plane sphere collision (still need to check min and max x and y values)
	//check if within plane
	bool Wall::collision(SceneNode* obj, float boundRad, glm::vec3* norm)
	{
		*norm = normal;			//return the normal of the plane
		glm::vec3 wallObjVec = node->getAbsolutePosition() - obj->getAbsolutePosition();
		float prod = abs(glm::dot(wallObjVec, normal));

		glm::vec3 pos = obj->getAbsolutePosition();
		glm::vec3 myPos = node->getAbsolutePosition();

		// SIDE WALLS , FRONT WALLS , FLOOR
		if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0)) 
		{ return prod <= boundRad && pos.y >= myPos.y - length/2 && pos.y <= myPos.y + length/2 && pos.z >= myPos.z - width/2 && pos.z <= myPos.z + width/2; }

		else if (normal == glm::vec3(0, 0, 1) || normal == glm::vec3(0, 0, -1)) 
		{ return prod <= boundRad && pos.y >= myPos.y - length/2 && pos.y <= myPos.y + length/2 && pos.x >= myPos.x - width/2 && pos.x <= myPos.x + width/2; }

		else 
		{ return prod <= boundRad && pos.x >= myPos.x - width/2 && pos.x <= myPos.x + width/2 && pos.z >= myPos.z - length/2 && pos.z <= myPos.z + length/2; }
	}
}