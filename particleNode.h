#ifndef PARTICLENODE_H
#define PARTICLENODE_H

#include "scene_node.h"

//ROOM
namespace game
{
	class ParticleNode
	{
	public:
		ParticleNode(SceneNode *);
		~ParticleNode();

		void update(void);
		void startAnimate(glm::vec3 position, glm::quat orientation, double duration);
		void updatePosition(glm::vec3 position);
		SceneNode *getParticle(void);
		void deleteNode(void);

	private:
		double timer;
		double lasttime;
		SceneNode *particle;
	};
}
#endif PARTICLENODE_H