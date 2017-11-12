#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "scene_node.h"

// SPIDERS ARE CHARACTERS COLLIDABLES 
namespace game
{
	class Environment
	{
	public:
		Environment(SceneNode*);
		~Environment();

		SceneNode* floor;
	/*
		Spider(SceneNode*, SceneNode*, SceneNode*);
		~Spider();

		SceneNode* body;		// Spider Body
		SceneNode* leftLeg;		// Spider left leg
		SceneNode* rightLeg;	// Spider right leg

		void update();
		bool collision(SceneNode*);*/
	private:
	protected:
	};
}

#endif // ENVIRONMENT_H