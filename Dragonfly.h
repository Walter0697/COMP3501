#ifndef DRAGONFLY_H
#define DRAGONFLY_H

#include "Enemy.h"
#include "scene_node.h"

// DRAGONFLIES ARE COLLIDABLES AND CHARACTERS
namespace game
{
	class DragonFly : public Enemy
	{
	public:
		DragonFly(SceneNode*, SceneNode*, SceneNode*, SceneNode*);
		~DragonFly();

		SceneNode* body;
		SceneNode* leftWing;
		SceneNode* rightWing;
		SceneNode* legs;
		
		int timer;
 		bool upWingMovement;

		void update();
		bool collision(SceneNode*);

	private:
	protected:
	};
}
#endif //DRAGONFLY_H
