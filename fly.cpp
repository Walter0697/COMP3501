#include "fly.h"

namespace game 
{
	Fly::Fly(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture) : SceneNode(name, geometry, material, texture)
	{
		speed = 2.0;					// hardcode speed
		maxFireRate = 30;				// maximum fire rate
		fireRate = 0;					// fire rate handler
		visible = true;
	}

	Fly::~Fly(){}

	void Fly::Update()
	{
		fireRate--;

		for (int i = 0; i < rockets.size(); i++)
		{
			std::cout << GetPosition().y << std::endl;
			// when timer is 0 delete the rocket else update it
			if (rockets[i]->timer <= 0)
			{
				rockets[i]->visible = false;
				rockets.erase(rockets.begin() + i);
			}
			else { rockets[i]->Update(); }
		}
	}
} // namespace game;
