#ifndef DRAGONFLY_H
#define DRAGONFLY_H

#include "Collidable.h"
#include "Character.h"
#include "scene_node.h"

// DRAGONFLIES ARE COLLIDABLES CHARACTERS AND SCENENODES
namespace game
{
	class DragonFly : public Collidable , public Character , public SceneNode
	{
	public:
		DragonFly(std::string, const Resource*, const Resource*, const Resource*);
		~DragonFly();

		void Update();
		bool collision(SceneNode*);
	private:
	protected:
	};
}
#endif //DRAGONFLY_H