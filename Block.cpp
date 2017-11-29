#include "Block.h"

namespace game
{
	Block::Block(SceneNode* obj)
	{
		object = obj;					// Object

		drag = false;
		dragger = glm::vec3(0,0,0);
		speed = 1;
	}

	Block::~Block() {}

	void Block::update()
	{
		if (drag) {// Follow dragging position
			object->SetPosition(glm::vec3(dragger.x, dragger.y - 6.7, dragger.z));
		}
		else {// Fall
			if (object->GetPosition().y > 0) {
				object->SetPosition(object->GetPosition() - glm::vec3(0, speed, 0));
				speed += 1;
				if (object->GetPosition().y < 0) {
					object->SetPosition(glm::vec3(object->GetPosition().x, -20, object->GetPosition().z));
				}
			}
		}
	}

	bool Block::collision(SceneNode * obj, float boundRad)
	{
		glm::vec3 difference = object->getAbsolutePosition() - obj->getAbsolutePosition();
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad);
	}
} // namespace game;
