#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/SpriteNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <Core/Command.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/User.hpp>
#include <DataStructures/AVLTree.hpp>
#include <memory>
#include <array>
#include <queue>
#include "Core/World.hpp"

class InApp : public State
{
private:

public:
										InApp(StateStack& stack, Context context, World::Mode mode);
	virtual void						draw();
	virtual bool						update(sf::Time dt);
	virtual bool						handleEvent(const sf::Event& event);
private:
	World 								mWorld;
	User								mUser;
};