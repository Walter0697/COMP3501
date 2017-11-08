#include "Spider.h"

namespace game
{
	Spider::Spider(std::string name, const Resource* geom, const Resource* mat, const Resource* texture) : SceneNode(name, geom, mat, texture)
	{
		speed = 1.0;
		fireRate = 0;
		maxFireRate = 50;
		maxHealth = 30;
		health = maxHealth;
	}

	Spider::~Spider() {}

	void Spider::Update()
	{

	}

	bool Spider::collision(SceneNode* object)
	{
		return false;
	}
}