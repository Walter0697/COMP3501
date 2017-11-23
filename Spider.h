#ifndef SPIDER_H
#define SPIDER_H

#include "Enemy.h"
#include "scene_node.h"

// SPIDERS ARE CHARACTERS COLLIDABLES 
namespace game
{
	class Spider : public Enemy
	{
	public:
		Spider(SceneNode*, SceneNode*, SceneNode*);
		~Spider();

		SceneNode* body;		// Spider Body
		SceneNode* leftLeg;		// Spider left leg
		SceneNode* rightLeg;	// Spider right leg

		bool isMoving;
 -		bool legMovement;
 -		int timer;
 
		void update();
		bool collision(SceneNode*);
	private:
	protected:
	};
}

#endif // SPIDER_H
