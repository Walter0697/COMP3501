#ifndef HUMAN_H
#define HUMAN_H

#include "scene_node.h"
#include "Enemy.h"
#include "Collidable.h"

// HUMANS ARE SCENENODES CHARACTERS AND COLLIDABLES 
namespace game
{
	class Human : public SceneNode , public Enemy, public Collidable
	{
	public:
		Human(const std::string = "", const Resource* = 0, const Resource* = 0, const Resource* = 0);
		~Human();
		void Update();
		void Human::UpdateTarget(glm::vec3 targPos);
		void Human::UpdateOrientation(glm::quat orient);
		bool collision(SceneNode*);
	private:


	protected:
	};
}
#endif // !HUMAN_H