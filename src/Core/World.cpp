#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/World.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>

World::World(sf::RenderWindow& window, TextureHolder& textures):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
	mRootNode(nullptr)
{
	loadTextures();
	buildScene();
}

void World::update(sf::Time dt)
{
	while (!mCommandQueue.isEmpty())
		mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt);
}

void World::draw()
{
	mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
	return mCommandQueue;
}

void World::loadTextures()
{
	// mTextures.load(Textures::AppBackground, "assets/images/background.jpg");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer = std::make_unique<SceneNode>();
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	// // Add first object
	// auto firstObj = std::make_unique<TreeNode>(20, 20.f, sf::Color::White, sf::Color::Black);
	// firstObj->setPosition(600, 100);
	// mRootNode = firstObj.get();
	// mSceneLayers[Objects]->attachChild(std::move(firstObj));
}