#pragma once
#include <SFML/Graphics.hpp>
#include <Core/Category.hpp>
#include <cassert>

class SceneNode;

struct Command
{
    std::function<void(SceneNode&, sf::Time)>   action;
    unsigned int                                category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=] (SceneNode& node, sf::Time dt)
	{
		assert(dynamic_cast<GameObject*>(&node) != nullptr);
		fn(static_cast<GameObject&>(node), dt);
	};
}
