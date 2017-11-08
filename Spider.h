#ifndef SPIDER_H
#define SPIDER_H

#include "Collidable.h"
#include "Character.h"
#include "scene_node.h"

// SPIDERS ARE CHARACTERS COLLIDABLES AND SCENENODES
namespace game
{
	class Spider : public Collidable , public Character, public SceneNode
	{
	public:
		Spider(std::string, const Resource*, const Resource*, const Resource*);
		~Spider();

		void Update();
		bool collision(SceneNode*);
	private:
	protected:
	};
}

#endif // SPIDER_H