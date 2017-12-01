#ifndef ROOM_H
#define ROOM_H

#include "scene_node.h"
#include "Collidable.h"
#include "wall.h"

//ROOM
namespace game
{
	class Room : public Collidable
	{
	public:
		Room();
		~Room();

		void addWall(Wall*);
		bool collision(SceneNode*, float);
		bool collision(SceneNode*, float, glm::vec3*);
		void SetFloor(Wall*);

	private:
		std::vector<Wall*> walls;
		Wall* floor;
	};
}
#endif ROOM_H