#include "particleNode.h"
#include "time.h"

namespace game
{
	ParticleNode::ParticleNode(SceneNode *part) {
		particle = part; 
		particle->SetVisible(false);
	}
	ParticleNode::~ParticleNode() {}

	void ParticleNode::update() 
	{
		if (timer > 0)
		{
			timer -= (glfwGetTime() - lasttime);
			lasttime = glfwGetTime();
			if (timer < 0)
				particle->SetVisible(false);
		}
	}

	void ParticleNode::startAnimate(glm::vec3 position, glm::quat orientation, double duration) 
	{
		particle->SetPosition(position);
		particle->SetOrientation(orientation);
		particle->updateTime();
		particle->SetVisible(true);
		timer = duration;
		lasttime = glfwGetTime();
	}
}