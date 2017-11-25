#ifndef CHARACTER_H
#define CHARACTER_H

#include "Collidable.h"

// CREATE A GENERAL CHARACTER WHICH WILL BE INHERITED BY ALL CHARACTER TYPES 
namespace game
{
	class Character : public Collidable
	{
	public:
		float speed;
		int fireRate;
		int maxFireRate;
		int health;
		int maxHealth;
		float boundingRadius;

		virtual void update() = 0;

	private:
	protected:
	};
}
#endif // CHARACTER_H