#include "fly.h"

namespace game 
{
	Fly::Fly(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture)
	{
		speed = 2.0;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		visible = true;
		maxHealth = 100;
		health = maxHealth;
	}

	Fly::~Fly(){}

	void Fly::Update()
	{
		fireRate--;

		for (int i = 0; i < rockets.size(); i++)
		{
			// when timer is 0 delete the rocket
			if (rockets[i]->timer <= 0)
			{
				rockets[i]->visible = false;
				rockets.erase(rockets.begin() + i);
			}
		}
	}

	bool Fly::collision(SceneNode * object)
	{
		return false;
	}
} // namespace game;
