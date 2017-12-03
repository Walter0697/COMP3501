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

	bool Wall::collision(SceneNode* obj, float boundRad, float) { return false; }

	//plane sphere collision (still need to check min and max x and y values)
	//check if within plane
	bool Wall::collision(SceneNode* obj, float boundRad, float off, glm::vec3* norm)
	{
		*norm = normal;			//return the normal of the plane

		//obj up vector
		glm::vec3 objUpVec = glm::normalize (glm::vec3(0, 1, 0) * obj->getAbsoluteOrientation());
		glm::vec3 objRealCenter = obj->getAbsolutePosition() + objUpVec * off;

		glm::vec3 wallObjVec = node->getAbsolutePosition() - objRealCenter;
		float prod = abs(glm::dot(wallObjVec, normal));

		glm::vec3 myPos = node->getAbsolutePosition();
		
		// SIDE WALLS , FRONT WALLS , FLOOR
		if (normal == glm::vec3(1, 0, 0) || normal == glm::vec3(-1, 0, 0)) 
		{ return prod <= boundRad && objRealCenter.y >= myPos.y - length/2 && objRealCenter.y <= myPos.y + length/2 && objRealCenter.z >= myPos.z - width/2 && objRealCenter.z <= myPos.z + width/2; }

		else if (normal == glm::vec3(0, 0, 1) || normal == glm::vec3(0, 0, -1)) 
		{ return prod <= boundRad && objRealCenter.y >= myPos.y - length/2 && objRealCenter.y <= myPos.y + length/2 && objRealCenter.x >= myPos.x - width/2 && objRealCenter.x <= myPos.x + width/2; }

		else 
		{ return prod <= boundRad && objRealCenter.x >= myPos.x - width/2 && objRealCenter.x <= myPos.x + width/2 && objRealCenter.z >= myPos.z - length/2 && objRealCenter.z <= myPos.z + length/2; }
	}
}