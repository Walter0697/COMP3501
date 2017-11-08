#ifndef SPIDER_H
#define SPIDER_H

#include "Collidable.h"
#include "Enemy.h"
#include "scene_node.h"

// SPIDERS ARE CHARACTERS COLLIDABLES AND SCENENODES
namespace game
{
	class Spider : public Collidable , public Enemy, public SceneNode
	{
	public:
		Spider(std::string, const Resource*, const Resource*, const Resource*);
		~Spider();

		void Update();
		void Spider::UpdateTarget(glm::vec3 targPos);
		void Spider::UpdateOrientation(glm::quat orient);
		bool collision(SceneNode*);
	private:
	protected:
	};
}

#endif // SPIDER_H