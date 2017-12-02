#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "scene_node.h"
#include "Collidable.h"
#include "room.h"

// SPIDERS ARE CHARACTERS COLLIDABLES 
namespace game
{
	class Environment : Collidable
	{
	public:
		Environment();
		Environment(SceneNode*);
		~Environment();

		glm::vec3 normals;
		bool collision(SceneNode* , float);
		bool collision(SceneNode*, float, glm::vec3);
		void addRoom(Room*);

	private:
		std::vector<Room*> rooms;
		SceneNode *floor;  //should remove later

		SceneNode *sky;

	protected:
	};
}

#endif ENVIRONMENT_H// ENVIRONMENT_H