#include "Rocket.h"
#include <iostream>

namespace game
{
	Rocket::Rocket(SceneNode* node , glm::vec3 dir)
	{
		this->direction = glm::normalize(dir);
		this->speed = 3.0;				// Hardcode speed 
		this->timer = 200;				// Hardcoded timer to get rid of rocket
		rocketNode = node;
	}

	Rocket::~Rocket(){}

	// Update rocket
	void Rocket::Update() 
	{ 
		timer--;	// Decrement timer 
		// If timer is zero then we should delete the bullet else update the position of the bullet 
		if (timer <= 0) { rocketNode->del = true; }	
		else { rocketNode->Translate(glm::vec3(speed * direction)); }
	}

	// Collision between rocket and other collidables USE ABSOLUTE POSITION
	bool Rocket::collision(SceneNode* collidable , Camera* camera_)
	{
		//when at a certain distance do ray sphere collision detection
		glm::vec3 apos, mpos, dif;
		apos = collidable->GetPosition();
		mpos = rocketNode->GetPosition();
		dif = mpos - apos;

		if ((((std::sqrt(std::pow(dif[0], 2) + std::pow(dif[1], 2) + std::pow(dif[2], 2))) <= 5.0)))
		{
			glm::vec3 l;
			l = glm::normalize(rocketNode->GetPosition() - collidable->GetPosition());
			float theta = acos(glm::dot(l , camera_->GetForward()));
			float test = tan(theta) * glm::length(rocketNode->GetPosition() - collidable->GetPosition());
			if (test < 0.0 && test > -1.0) return true;
		}
		return false;
	}

	bool Rocket::collision(SceneNode* object)
	{
		return false;
	}
}