#ifndef DRAGONFLY_H
#define DRAGONFLY_H

#include "Collidable.h"
#include "Enemy.h"
#include "scene_node.h"

// DRAGONFLIES ARE COLLIDABLES CHARACTERS AND SCENENODES
namespace game
{
	class DragonFly : public Collidable , public Enemy, public SceneNode
	{
	public:
		DragonFly(std::string, const Resource*, const Resource*, const Resource*);
		~DragonFly();

		void Update();
		void DragonFly::UpdateTarget(glm::vec3 targPos);
		void DragonFly::UpdateOrientation(glm::quat orient);
		bool collision(SceneNode*);
	private:
	protected:
	};
}
#endif //DRAGONFLY_H