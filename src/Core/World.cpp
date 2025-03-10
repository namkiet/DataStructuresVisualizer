#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/World.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "Core/Container.hpp"
World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
	mRootNode(nullptr),
	ButtonList()
{
	 sf::Vector2f insertButtonPos(20.f, window.getSize().y - ButtonSize.y - 20.f); 
    sf::Vector2f deleteButtonPos(20.f, insertButtonPos.y - ButtonSize.y - 10.f);

	Button::Ptr InsertButton = std::make_shared<Button>(fonts, textures, insertButtonPos);
	Button::Ptr DeleteButton = std::make_shared<Button>(fonts, textures, deleteButtonPos);
    InsertButton->setText("Insert");
    DeleteButton->setText("Delete");


    ButtonList.addButton(InsertButton);
    ButtonList.addButton(DeleteButton);


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
	mWindow.draw(ButtonList);
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
    SceneNode::Ptr layer(new SceneNode());
    mSceneLayers[i] = layer.get(); // add layers to mSceneLayers
    mSceneGraph.attachChild(std::move(layer));
 }
}

void World::handleEvent(const sf::Event& event){
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
    {
		ButtonList.handleEvent(event);
    }
}