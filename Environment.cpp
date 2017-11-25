#include "Environment.h"

namespace game
{
	Environment::Environment(SceneNode *floor) 
	{
		this->floor = floor;
	}

	Environment::~Environment() {}

	bool Environment::collision(SceneNode* object , float boundRad)
	{
		return false;
	}

}