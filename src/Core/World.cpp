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
	OperationButtonsList(std::make_shared<GUI::Container>()),
	mMode(World::Mode::None),
	mFont(fonts),
	ModeContainer(std::make_shared<GUI::Container>())
	// mAVL()
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
	mWindow.draw(*OperationButtonsList);
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

void World::setMode(World::Mode mode){
	ModeContainer->ChangeActivateChild(mode); // ensure that the Modecontainer handle the true index of activated child
	OperationButtonsList->makeEmpty();
	if(mode == World::Mode::AVL)
	{
		// mRootNode = std::make_unique<AVLTree>(); 
		// mRootNode->setPosition(sf::Vector2f(600,100));
   		// mSceneLayers[Tree]->attachChild(std::move(mRootNode));
		std::unique_ptr<AVLTree> root(new AVLTree());
		mAVL = root.get();
		mAVL->setPosition(sf::Vector2f(600, 100));
		mSceneLayers[Tree]->attachChild(std::move(root));



		/* FUNCTIONAL BUTTONS */
		sf::Vector2f insertButtonPos(20.f, mWindow.getSize().y - GUI::ButtonSize.y - 20.f); 
		GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), insertButtonPos, "Insert");
		sf::Vector2f inputBoxInsertPos(insertButtonPos.x + 150 + 10.f, insertButtonPos.y);
		GUI::DeliverTextBox::Ptr InputBoxInsert = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main),inputBoxInsertPos,sf::Vector2f(100.f, 40.f));
		InputBoxInsert->setButtonParent(InsertButton);
		InsertButton->setCallback([this,InsertButton](){
			if(InsertButton->getSubComponentInfo().num != -1){
				this->mAVL->insert(InsertButton->getSubComponentInfo().num);
				InsertButton->setSubComponentInfo(-1);
				std::cout<<"Insert ok";
			}
			else if(InsertButton->getSubComponentInfo().VecNum.size() != 0){
				for(auto& element: InsertButton->getSubComponentInfo().VecNum){
					this->mAVL->insert(element);
				}
			}
		});

		sf::Vector2f deleteButtonPos(20.f, insertButtonPos.y - GUI::ButtonSize.y - 10.f);
		GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), deleteButtonPos, "Delete");
		sf::Vector2f inputBoxDeletePos(deleteButtonPos.x + 150 + 10.f, deleteButtonPos.y);
		GUI::DeliverTextBox::Ptr InputBoxDelete = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main), inputBoxDeletePos, sf::Vector2f(100.f, 40.f));
		InputBoxDelete->setButtonParent(DeleteButton);
		DeleteButton->setCallback([this,DeleteButton](){
			if(DeleteButton->getSubComponentInfo().num != -1){
				std::cout<<"Delete num"<<std::endl;
			}
			else if(DeleteButton->getSubComponentInfo().VecNum.size() != 0){
				for(auto& element: DeleteButton->getSubComponentInfo().VecNum){
					std::cout<<"Delete list of num"<<std::endl;
				}
			}
		});
		
		// Add search Button

		InsertButton->addSubComponent(InputBoxInsert);

		// InsertButton->setCallback([this]) {
		// 	int val = // insertButton->DeliverTextBox->getValue;
		// 	ds.insert(val);
		// }

		DeleteButton->addSubComponent(InputBoxDelete);
		OperationButtonsList->pack(InsertButton);
		OperationButtonsList->pack(DeleteButton);
	}
	// 3 other data structures
}

void World::handleEvent(const sf::Event& event){

	ModeContainer->handleEvent(event);
	OperationButtonsList->handleEvent(event);

	// if (event.type == sf::Event::KeyPressed)
    // {
    //     if (event.key.code == sf::Keyboard::A)
    //     {
    //         int value = std::rand() % 100;
    //         // int value = v[id];
    //         // id++;
    //         std::cerr << value << "\n";
    //         mAVL->insert(value);
    //     }

    //     // if (event.key.code == sf::Keyboard::B)
    //     //     mAVL->setPosition(mAVL->getPosition() - sf::Vector2f(50, 0));

    //     // if (event.key.code == sf::Keyboard::C)
    //     //     mAVL->leftRotate();

    //     if (event.key.code == sf::Keyboard::D)
    //     {
    //         if (mAVL->search(5))
    //             std::cerr << "Found \n";
    //         else 
    //             std::cerr << "Cannot find \n";
    //     }
    // }
}