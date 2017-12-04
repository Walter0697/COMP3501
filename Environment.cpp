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

	bool Environment::collision(SceneNode*, float, float) { return false; }

	//need normals of each plane I create 
	bool Environment::collision(SceneNode* object , float boundRad , float off, glm::vec3* norm)
	{
		for (int w = 0; w < rooms.size(); w++)
		{
			if (rooms[w]->collision(object, boundRad, off, norm)) return true;
		}
		return false;
	}

	void Environment::addRoom(Room *myRoom)
	{
		rooms.push_back(myRoom);
	}

}