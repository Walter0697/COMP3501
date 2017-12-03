#include "particleNode.h"

namespace game
{
	ParticleNode::ParticleNode(SceneNode *part) {
		particle = part; 
		particle->SetVisible(false);
	}
	ParticleNode::~ParticleNode() {}

	void ParticleNode::update() {}
	void ParticleNode::startAnimate(glm::vec3 position) 
	{
		particle->SetPosition(position);
		particle->updateTime();
		particle->SetVisible(true);
	}
}