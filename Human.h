#ifndef HUMAN_H
#define HUMAN_H

#include "scene_node.h"
#include "Character.h"
#include "Collidable.h"

// HUMANS ARE SCENENODES CHARACTERS AND COLLIDABLES 
namespace game
{
	class Human : public SceneNode , public Character, public Collidable
	{
	public:
		Human(const std::string = "", const Resource* = 0, const Resource* = 0, const Resource* = 0);
		~Human();
		void Update();
		bool collision(SceneNode*);
	private:
	protected:
	};
}
#endif // !HUMAN_H