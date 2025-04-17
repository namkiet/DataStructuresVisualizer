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
#include "DataStructures/Graph.hpp"
#include "Core/Utility.hpp"
World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
	mMode(World::Mode::NoneMode),
	BackRequest(false),
	mTextures(textures),
	mFonts(fonts)
{
	textures.get(Textures::Pause).setSmooth(true);
	mPause.setTexture(textures.get(Textures::Pause));
	textures.get(Textures::Play).setSmooth(true);
	mPlay.setTexture(textures.get(Textures::Play));

    buildScene();
	// mShader.loadFromFile("shader/colormode.frag", sf::Shader::Fragment);
}

void World::update(sf::Time dt)
{
	mSceneGraph.update(dt);

	mPseudoCode->setCode(mDataStructure->getCode());
	mPseudoCode->setStep(mDataStructure->getStep());

	mInfoPanel->setText(mDataStructure->getInfo());

	mProgressBar->setProgress(mDataStructure->getProgress());

	if (mDataStructure->stop && !mDataStructure->isRunning())
		mPauseButton->setSprite(mPlay);
	else
		mPauseButton->setSprite(mPause);

	if (mDataStructure->canUndo())
	{
		mFirstButton->enable();
		mPrevButton->enable();
	}
	else
	{
		mFirstButton->disable();
		mPrevButton->disable();
	}

	if (mDataStructure->canRedo())
	{
		mLastButton->enable();
		mNextButton->enable();
	}
	else
	{
		mLastButton->disable();
		mNextButton->disable();
	}
}

void World::draw()
{
	mWindow.draw(background);
	mWindow.draw(mSceneGraph);

	mWindow.draw(*mFirstButton);
	mWindow.draw(*mLastButton);
	mWindow.draw(*mPrevButton);
	mWindow.draw(*mNextButton);
	mWindow.draw(*mPauseButton);
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

	ProgressBar::Ptr progress(new ProgressBar(SCREEN::Width - 700, SCREEN::Height - 35, 500, 20));
	mProgressBar = progress.get();
	mSceneLayers[Progress]->attachChild(std::move(progress));

	sf::Font emptyFont;

	mTextures.get(Textures::First).setSmooth(true);
	sf::Sprite firstSprite(mTextures.get(Textures::First));
	mFirstButton = std::make_shared<GUI::Button>(emptyFont, sf::Vector2f(SCREEN::Width - 870, SCREEN::Height - 35), "", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mFirstButton->setSprite(firstSprite);
	mFirstButton->setToggle(false);

	mTextures.get(Textures::Last).setSmooth(true);
	sf::Sprite lastSprite(mTextures.get(Textures::Last));
	mLastButton = std::make_shared<GUI::Button>(emptyFont, sf::Vector2f(SCREEN::Width - 750, SCREEN::Height - 35), "", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mLastButton->setSprite(lastSprite);
	mLastButton->setToggle(false);

	mTextures.get(Textures::Prev).setSmooth(true);
	sf::Sprite prevSprite(mTextures.get(Textures::Prev));
	mPrevButton = std::make_shared<GUI::Button>(emptyFont, sf::Vector2f(SCREEN::Width - 840, SCREEN::Height - 35), "", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mPrevButton->setSprite(prevSprite);
	mPrevButton->setToggle(false);

	mTextures.get(Textures::Next).setSmooth(true);
	sf::Sprite nextSprite(mTextures.get(Textures::Next));
	mNextButton = std::make_shared<GUI::Button>(emptyFont, sf::Vector2f(SCREEN::Width - 780, SCREEN::Height - 35), "", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mNextButton->setSprite(nextSprite);
	mNextButton->setToggle(false);
	
	mPauseButton = std::make_shared<GUI::Button>(emptyFont, sf::Vector2f(SCREEN::Width - 810, SCREEN::Height - 35), "", sf::Vector2f(20, 20), GUI::Button::ShapeType::Circle, GUI::Button::ContentType::Image);
	mPauseButton->setSprite(mPause);	
	mPauseButton->setToggle(false);

	Textures::ID currentBackground = getCurrentBackgroundOption();
	sf::Texture &bg = mTextures.get(currentBackground);
	background.setTexture(bg);
	sf::Color bgcolor = background.getColor();
	bgcolor.a = 240;
	background.setColor(bgcolor);
}

void World::setMode(World::Mode mode)
{
	mMode = mode;
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
		std::unique_ptr<Graph> graph(new Graph());
		mDataStructure = graph.get();
		mSceneLayers[DataStructure]->attachChild(std::move(graph));
	}

	mSceneLayers[DataStructure]->setPosition(VIZ::DS::Position);
	mMainUI->createButtonList(mode, mDataStructure);


	mPrevButton->setCallback([=](){ mDataStructure->undo(); });
	mNextButton->setCallback([=](){ mDataStructure->redo(); });
	mPauseButton->setCallback([=]() { mDataStructure->run(); });
	mFirstButton->setCallback([=](){ mDataStructure->toFirst(); });
	mLastButton->setCallback([=](){ mDataStructure->toLast(); });
}

void World::handleEvent(const sf::Event& event)
{
	mFirstButton->handleEvent(event);
	mLastButton->handleEvent(event);
	mPrevButton->handleEvent(event);
	mNextButton->handleEvent(event);
	mPauseButton->handleEvent(event);

	mMainUI->handleEvent(event);

	if(mMode == World::Mode::GraphMode)
	{
		auto mGraph = static_cast<Graph*>(mDataStructure);
		// std::cout<<"In world can jump to graph handle event"<<std::endl;
		mGraph->handleEvent(event);
	}
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

		if (event.key.code == sf::Keyboard::S)
		{
			mDataStructure->run();
		}
	}

	if (mProgressBar->handleEvent(event))
	{
		mDataStructure->loadStep(mProgressBar->getProgress());	
	}
}
 
void World::updateBackRequest()
{
	BackRequest = mMainUI->getBackRequest();
}

bool World::getBackRequest()
{
	return BackRequest;
}
