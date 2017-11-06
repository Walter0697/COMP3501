#ifndef HUMAN_H
#define HUMAN_H

#include "scene_node.h"
#include "Character.h"

namespace game
{
	class Human : public SceneNode , public Character
	{
	public:
		Human(const std::string = "", const Resource* = 0, const Resource* = 0, const Resource* = 0);
		~Human();
		void Update();

	private:
	protected:
	};
}
#endif // !HUMAN_H