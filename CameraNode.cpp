#include "CameraNode.h"

namespace game
{
	CameraNode::CameraNode(Camera* cam) : SceneNode("Camera" , 0 , 0, 0)
	{
		this->camera = cam;
		visible = false;
	}

	CameraNode::~CameraNode() {}

	/*
	glm::vec3 CameraNode::GetPosition(void) const
	{
		return camera->GetPosition();
	}
	*/

	// glm::vec3 CameraNode::

	// MIGHT NEED TO BE CHANGED !!!!!!!!
	glm::vec3 CameraNode::GetPosition(void) const
	{
		if (camera->firstPerson) { return camera->GetPosition() - (camera->GetForward() * camera->distance); }
		else { return camera->GetPosition(); }
	}

	glm::quat CameraNode::GetOrientation(void) const { return camera->GetOrientation(); }
}// Game namespace 