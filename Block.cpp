#include "Block.h"

namespace game
{
	Block::Block(SceneNode* obj)
	{
		object = obj;					// Object
		boundingRadius = 0.6;

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

	bool Block::collision(SceneNode * obj, float off, float boundRad)
	{
		//find real center of the object
		glm::vec3 objUpVec = glm::normalize(obj->GetOrientation() * glm::vec3(0, 1, 0));
		glm::vec3 objRealCenter = obj->getAbsolutePosition() + objUpVec * off;

		//find my real center
		glm::vec3 myUpVec = object->GetOrientation() * glm::vec3(0, 1, 0);
		glm::vec3 myRealCenter = object->getAbsolutePosition() + myUpVec * offset;

		//find difference in positions
		glm::vec3 difference = myRealCenter - objRealCenter;

		//compare distances 
		return ((std::sqrt(std::pow(difference[0], 2) + std::pow(difference[1], 2) + std::pow(difference[2], 2))) <= boundRad + boundingRadius);
	}
} // namespace game;
