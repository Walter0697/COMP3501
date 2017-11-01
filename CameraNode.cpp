#include "CameraNode.h"

namespace game
{
	CameraNode::CameraNode(Camera* cam) : SceneNode("Camera" , 0 , 0, 0)
	{
		this->camera = cam;
		visible = false;
	}

	glm::vec3 CameraNode::GetPosition(void) const
	{
		return camera->GetPosition();
	}

	glm::quat CameraNode::GetOrientation(void) const
	{
		return camera->GetOrientation();
	}

	CameraNode::~CameraNode() {}
}