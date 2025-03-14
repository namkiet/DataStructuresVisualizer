#include <SFML/Graphics/RenderWindow.hpp>
#include <Core/World.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "GUI/Container.hpp"
#include "DataStructures/AVLTree.hpp"
#include "GUI/ExpandableButton.hpp"

World::World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
	OperationButtonsList(),
	mMode(World::Mode::None),
	mFont(fonts),
	ModeContainer(std::make_shared<GUI::Container>())
{
	CreateModeContainer();
	// mRootNode = std::move(mRootNode);
    loadTextures();
    buildScene();
}

void World::CreateModeContainer(){

	float buttonX = 20.f;
	sf::Vector2f avlButtonPos(buttonX, 20.f);
	sf::Vector2f linkedListButtonPos(buttonX, avlButtonPos.y + GUI::ButtonSize.y + 10.f);
	sf::Vector2f heapButtonPos(buttonX, linkedListButtonPos.y + GUI::ButtonSize.y + 10.f);
	sf::Vector2f graphButtonPos(buttonX, heapButtonPos.y + GUI::ButtonSize.y + 10.f);

	GUI::Button::Ptr AVLButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), avlButtonPos, "AVL");
	AVLButton->setCallback([this]() { setMode(World::Mode::AVL); });
	GUI::Button::Ptr LinkedListButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), linkedListButtonPos, "LinkedList");
	LinkedListButton->setCallback([this]() { setMode(World::Mode::LinkedList); });
	GUI::Button::Ptr HeapButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), heapButtonPos, "Heap");
	HeapButton->setCallback([this]() { setMode(World::Mode::Heap); });
	GUI::Button::Ptr GraphButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), graphButtonPos, "Graph");
	GraphButton->setCallback([this]() { setMode(World::Mode::Graph); });

	ModeContainer->pack(AVLButton);
	ModeContainer->pack(LinkedListButton);
	ModeContainer->pack(HeapButton);
	ModeContainer->pack(GraphButton);
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
	mWindow.draw(*ModeContainer);
	for (auto& Operation: OperationButtonsList){
		mWindow.draw(*Operation);
	}
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

	std::unique_ptr<AVLTree> root(new AVLTree());
	mAVL = root.get();
	mSceneLayers[Tree]->attachChild(std::move(root));
	mSceneLayers[Tree]->setPosition(sf::Vector2f(1000, 100));

	// v = {44, 45, 42, 40, 41};
	// v = {3, 7, 12, 18, 25, 31, 39, 42, 50, 57, 63, 69, 75, 81, 88, 94, 99, 105, 112, 118, 124, 130, 137, 143, 150, 157, 163, 170, 176, 183};
	v = {144, 127, 179, 160, 187, 167};
	id = 0;
}

void World::setMode(World::Mode mode){
	OperationButtonsList.clear();
	
	if(mode == World::Mode::AVL)
	{
		// mRootNode = std::make_unique<AVLTree>(); 
		// mRootNode->setPosition(sf::Vector2f(600,100));
   		// mSceneLayers[Tree]->attachChild(std::move(mRootNode));

		// std::unique_ptr<HeapTree> root(new HeapTree());
		// mAVL = root.get();
		// mAVL->setPosition(sf::Vector2f(600, 100));
		// mSceneLayers[Tree]->attachChild(std::move(root));

		/* FUNCTIONAL BUTTONS */
		sf::Vector2f insertButtonPos(20.f, mWindow.getSize().y - GUI::ButtonSize.y - 20.f); 
		GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), insertButtonPos, "Insert");
		sf::Vector2f inputBoxInsertPos(insertButtonPos.x + 150 + 10.f, insertButtonPos.y);
		GUI::TextBox::Ptr InputBoxInsert = std::make_shared<GUI::TextBox>(mFont.get(Fonts::ID::Main),inputBoxInsertPos,sf::Vector2f(100.f, 40.f));

		sf::Vector2f deleteButtonPos(20.f, insertButtonPos.y - GUI::ButtonSize.y - 10.f);
		GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), deleteButtonPos, "Delete");
		sf::Vector2f inputBoxDeletePos(deleteButtonPos.x + 150 + 10.f, deleteButtonPos.y);
		GUI::TextBox::Ptr InputBoxDelete = std::make_shared<GUI::TextBox>(mFont.get(Fonts::ID::Main), inputBoxDeletePos, sf::Vector2f(100.f, 40.f));

		
		// Add search Button

		InsertButton->addSubComponent(InputBoxInsert);

		// InsertButton->setCallback([this]) {
		// 	int val = // insertButton->textBox->getValue;
		// 	ds.insert(val);
		// }

		DeleteButton->addSubComponent(InputBoxDelete);
		OperationButtonsList.push_back(InsertButton);
		OperationButtonsList.push_back(DeleteButton);
	}
	// 3 other data structures
}

void World::handleEvent(const sf::Event& event){

	ModeContainer->handleEvent(event);
	for(auto& Operation: OperationButtonsList) {
		Operation->handleEvent(event);
	}

	if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::A)
        {
            // int value = std::rand() % 200;
            int value = v[id];
            id++;
            std::cerr << value << "\n";
            mAVL->insert(value);
        }

        // if (event.key.code == sf::Keyboard::B)
        //     mAVL->setPosition(mAVL->getPosition() - sf::Vector2f(50, 0));

        // if (event.key.code == sf::Keyboard::C)
        //     mAVL->leftRotate();

        if (event.key.code == sf::Keyboard::D)
        {
            if (mAVL->search(5))
                std::cerr << "Found \n";
            else 
                std::cerr << "Cannot find \n";
        }
    }
}