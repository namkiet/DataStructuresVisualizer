#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/World.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "GUI/Container.hpp"
#include "DataStructures/AVLTree.hpp"
#include "GUI/ExpandableButton.hpp"
#include <Core/Variables.hpp>
#include "GUI/MainUI.hpp"
#include "DataStructures/LinkedList.hpp"

World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
	mMode(World::Mode::NoneMode),
	BackRequest(false),
	mTextures(textures),
	mFonts(fonts)
{
    buildScene();
	// mShader.loadFromFile("shader/colormode.frag", sf::Shader::Fragment);
}

void World::update(sf::Time dt)
{
	mSceneGraph.update(dt);

	mPseudoCode->setCode(mDataStructure->getCode());
	mPseudoCode->setStep(mDataStructure->getStep());

	mInfoPanel->setText(mDataStructure->getInfo());
}

void World::draw()
{
	mWindow.draw(background);
	mWindow.draw(mSceneGraph);
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

	PseudoCode::Ptr pseudo(new PseudoCode(mFonts.get(Fonts::ID::Main)));
	pseudo->setPosition(UI::CODEBOX::Position);
	mPseudoCode = pseudo.get();
	mSceneLayers[CodeBox]->attachChild(std::move(pseudo));

	InfoPanel::Ptr panel(new InfoPanel(mFonts.get(Fonts::ID::Main), UI::INFOBOX::Size));
	panel->setPosition(UI::INFOBOX::Position);
	mInfoPanel = panel.get();
	mSceneLayers[InfoBox]->attachChild(std::move(panel));
	
	MainUI::Ptr mainUI(new MainUI(mTextures, mFonts));
	mMainUI = mainUI.get();
	mSceneLayers[SidePanel]->attachChild(std::move(mainUI));

	ProgressBar::Ptr progress(new ProgressBar(100, 100, 100, 20));
	mProgressBar = progress.get();
	mSceneLayers[Progress]->attachChild(std::move(progress));

	sf::Texture &bg = mTextures.get(Textures::AppBackground);
	background.setTexture(bg);
	sf::Color bgcolor = background.getColor();
	bgcolor.a = 240;
	background.setColor(bgcolor);
}

void World::setMode(World::Mode mode)
{
	if (mode == World::Mode::AVLMode)
	{
		std::unique_ptr<AVLTree> avl(new AVLTree());
		mDataStructure = avl.get();
		mSceneLayers[DataStructure]->attachChild(std::move(avl));
	}
	else if (mode == World::Mode::LinkedListMode)
	{
		std::unique_ptr<LinkedList> ll(new LinkedList());
		mDataStructure = ll.get();
		mSceneLayers[DataStructure]->attachChild(std::move(ll));
	}
	else if (mode == World::Mode::HeapMode)
	{
		std::unique_ptr<HeapTree> heap(new HeapTree());
		mDataStructure = heap.get();
		mSceneLayers[DataStructure]->attachChild(std::move(heap));
	}
	else if (mode == World::Mode::GraphMode)
	{
		// mDataStructure = new Graph();
	}

	mSceneLayers[DataStructure]->setPosition(VIZ::DS::Position);
	mMainUI->createButtonList(mode, mDataStructure);
}

void World::handleEvent(const sf::Event& event)
{

	mMainUI->handleEvent(event);
	updateBackRequest();

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::A) {
			ANIMATION::Speed *= 1.2;
			if (ANIMATION::Speed >= 10) 
			ANIMATION::Speed = 0.2f;
		}
		if (event.key.code == sf::Keyboard::U) {
			mDataStructure->undo();
		}
		if (event.key.code == sf::Keyboard::R) {
			mDataStructure->redo();
		}
	}

	mProgressBar->handleEvent(event);
}

void World::updateBackRequest()
{
	BackRequest = mMainUI->getBackRequest();
}

bool World::getBackRequest()
{
	return BackRequest;
}
