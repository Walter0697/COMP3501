#include "DragonFly.h"

namespace game
{
	/* Constructor */
	DragonFly::DragonFly(std::string name, const Resource* geom, const Resource* mat, const Resource* texture) : SceneNode(name, geom, mat, texture)
	{
		speed = 3.0;
		fireRate = 0;
		maxFireRate = 90;
		maxHealth = 20;
		health = maxHealth;
	}

	/* Destructor */
	DragonFly::~DragonFly() {}

	/* Update */
	void DragonFly::Update()
	{

	}

	/* Collision */
	bool DragonFly::collision(SceneNode* object)
	{
		return false;
	}
}