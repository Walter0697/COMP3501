#include "room.h"

namespace game
{
	Room::Room() {}
	Room::~Room() {}

	//add wall to wall vector
	void Room::addWall(Wall* myWall)
	{
		walls.push_back(myWall);
	}

	bool Room::collision(SceneNode*, float) { return false; }

	//check for collision on all walls in a room
	bool Room::collision(SceneNode* obj, float boundRad, glm::vec3* norm)
	{
		for (int i = 0; i < walls.size(); i++)
		{
			if (walls[i]->collision(obj, boundRad, norm)) return true;
		}
		return false;
	}

	void Room::SetFloor(Wall *myFloor)
	{
		floor = myFloor;
	}
}