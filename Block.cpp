#include "Block.h"

namespace game
{
	Block::Block(SceneNode* obj)
	{
		object = obj;					// Object
		boundingRadius = 2.0;			// bounding radius
		offset = 0.0;					// offset from center

		beingDragged = false;			// being dragged check
		onFloor = false;				// onFloor check
		gravity = -0.4f;				// gravity value
	}

	Block::~Block() {}

	void Block::update()
	{
		// not being dragged and not on the floor -> drop with gravity
		if (!beingDragged && !onFloor) { object->Translate(glm::vec3(0, gravity, 0)); }
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
