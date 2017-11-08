#include "Rocket.h"
#include <iostream>

namespace game
{
	Rocket::Rocket(const std::string name, const Resource *geometry, const Resource *material, const Resource *texture , glm::vec3 direction) : SceneNode (name, geometry, material,texture)
	{
		this->direction = glm::normalize(direction);
		this->speed = 3.0;				// Hardcode speed 
		this->timer = 200;				// Hardcoded timer to get rid of rocket
		visible = true;
	}

	Rocket::~Rocket(){}

	// Update rocket
	void Rocket::Update() 
	{ 
		timer--;	// Decrement timer 
		// If timer is zero then we should delete the bullet else update the position of the bullet 
		if (timer <= 0) { del = true; }	
		else { Translate(glm::vec3(speed * direction)); }
	}

	// Collision between rocket and other collidables USE ABSOLUTE POSITION
	bool Rocket::collision(SceneNode* collidable , Camera* camera_)
	{
		//when at a certain distance do ray sphere collision detection
		glm::vec3 apos, mpos, dif;
		apos = collidable->GetPosition();
		mpos = this->GetPosition();
		dif = mpos - apos;

		if ((((std::sqrt(std::pow(dif[0], 2) + std::pow(dif[1], 2) + std::pow(dif[2], 2))) <= 5.0)))
		{
			glm::vec3 l;
			l = glm::normalize(this->GetPosition() - collidable->GetPosition());
			float theta = acos(glm::dot(l , camera_->GetForward()));
			float test = tan(theta) * glm::length(this->GetPosition() - collidable->GetPosition());
			if (test < 0.0 && test > -1.0) return true;
		}
		return false;
	}

	bool Rocket::collision(SceneNode* object)
	{
		return false;
	}
}