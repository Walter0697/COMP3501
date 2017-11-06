#include "Human.h"

namespace game
{
	Human::Human(const std::string name, const Resource* geometry, const Resource* material, const Resource* texture) : SceneNode(name, geometry, material, texture)
	{
		speed = 3.0;		// Human speed of movement
		fireRate = 0;		// Human fireRate 
		maxFireRate = 30;	// Human maxFireRate
	}

	Human::~Human() {}

	void Human::Update() {}
}
