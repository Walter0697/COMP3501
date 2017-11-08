#ifndef CHARACTER_H
#define CHARACTER_H


#include <iostream>
#include <ctime>

// CREATE A GENERAL CHARACTER WHICH WILL BE INHERITED BY ALL CHARACTER TYPES 
namespace game
{
	class Character
	{
	public:
		float speed;
		int fireRate;
		int maxFireRate;
		int health;
		int maxHealth;

	private:
	protected:
	};
}
#endif // CHARACTER_H