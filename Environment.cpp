#include "Environment.h"
#include <iostream>

namespace game
{
	Environment::Environment() {}

	Environment::Environment(SceneNode *floor) 
	{
		this->floor = floor;
	}

	Environment::~Environment() {}

	bool Environment::collision(SceneNode*, float) { return false; }

	//need normals of each plane I create 
	bool Environment::collision(SceneNode* object , float boundRad , glm::vec3 direction)
	{
		glm::vec3 wallObjVec = floor->getAbsolutePosition() - object->getAbsolutePosition();
		float prod = abs(glm::dot(wallObjVec, glm::vec3(0,1,0)));
		std::cout << prod << std::endl;
		return prod <= boundRad;
	}

	void Environment::addRoom(Room *myRoom)
	{
		rooms.push_back(myRoom);
	}

}