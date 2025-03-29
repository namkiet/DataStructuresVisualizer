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
	// OperationButtonsList(std::make_shared<GUI::Container>()),
	mMode(World::Mode::NoneMode),
	mFont(fonts),
	ModeContainer(std::make_shared<GUI::Container>()),
	mMainUI(std::make_shared<GUI::MainUI>(window,textures,fonts)),
	BackRequest(false)
{
	// CreateModeContainer();
    // loadTextures();
    buildScene();

}

void World::CreateModeContainer()
{
// 	float buttonX = 20.f;	
// 	sf::Vector2f avlButtonPos(buttonX, 20.f);
// 	sf::Vector2f linkedListButtonPos(buttonX, avlButtonPos.y + GUI::ButtonSize.y + 10.f);
// 	sf::Vector2f heapButtonPos(buttonX, linkedListButtonPos.y + GUI::ButtonSize.y + 10.f);
// 	sf::Vector2f graphButtonPos(buttonX, heapButtonPos.y + GUI::ButtonSize.y + 10.f);
// 	GUI::Button::Ptr AVLButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), avlButtonPos, "AVL");
// 	AVLButton->setCallback([this]() { setMode(World::Mode::AVL); });

// 	GUI::Button::Ptr LinkedListButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), linkedListButtonPos, "LinkedList");
// 	LinkedListButton->setCallback([this]() { setMode(World::Mode::LinkedList); });
// 	GUI::Button::Ptr HeapButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), heapButtonPos, "Heap");
// 	HeapButton->setCallback([this]() { setMode(World::Mode::Heap); });
// 	GUI::Button::Ptr GraphButton = std::make_shared<GUI::Button>(mFont.get(Fonts::ID::Main), graphButtonPos, "Graph");
// 	GraphButton->setCallback([this]() { setMode(World::Mode::Graph); });

// 	ModeContainer->pack(AVLButton);
// 	ModeContainer->pack(LinkedListButton);
// 	ModeContainer->pack(HeapButton);
// 	ModeContainer->pack(GraphButton);
}


void World::update(sf::Time dt)
{
	while (!mCommandQueue.isEmpty())
		mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
	mSceneGraph.update(dt);

	mInfoPanel->setText(mDataStructure->getInfo());
}

void World::draw()
{

	// mWindow.draw(*ModeContainer);
	// mWindow.draw(*OperationButtonsList);

	// mMainUI->draw(mWindow, sf::RenderStates::Default);
	// mWindow.draw(mSceneGraph);
	// mWindow.draw(*ModeContainer);
	// mWindow.draw(*OperationButtonsList);
	// mWindow.draw(*mInfoPanel);


	mMainUI->draw(mWindow, sf::RenderStates::Default);
	mWindow.draw(mSceneGraph);
	if (mPseudoCode) mPseudoCode->draw(mWindow);
	
	// if (mPseudoCode) mPseudoCode->draw(mWindow);
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

// <<<<<<< HEAD
// 	std::unique_ptr<AVLTree> root(new AVLTree());
// 	mDataStructure = root.get();
// 	mSceneLayers[DataStructure]->attachChild(std::move(root));

// 	mSceneLayers[DataStructure]->setPosition(LAYOUT::DS::Position);

// 	mPseudoCode = new PseudoCode(mFont.get(Fonts::ID::Main), 700, 520);

	mInfoPanel = new GUI::InfoPanel(300, 200, sf::Vector2f(100, 300));
	mInfoPanel->setCharacterSize(30);

// 	mDataStructure->updateStepCallback = [this](int step) {
// 		this->mPseudoCode->setStep(step);
// 	};
// =======
	// std::unique_ptr<AVLTree> root(new AVLTree());
	// mDataStructure = root.get();
	
	// mSceneLayers[DataStructure]->attachChild(std::move(root));
	// mSceneLayers[DataStructure]->setPosition(sf::Vector2f(1000, 100));

	mPseudoCode = new PseudoCode(mFont.get(Fonts::ID::Main), 700, 520);

	// mDataStructure->updateStepCallback = [this](int step) {
	// 	this->mPseudoCode->setStep(step);
	// };

	// v = {42, 17, 93, 56, 81, 12, 65, 37, 29, 74, 8, 90, 33, 50, 22, 99, 5, 47, 86, 60, 15, 78, 3, 69, 25, 91, 40, 7, 88, 54, 31};
	// // v = {74, 12, 217, 36, 61, 77, 286, 153, 337, 93, 121, 47, 463, 248, 146};
	// // v = {10,9,8,7,6,5,4};
	// // v = {44, 45, 42, 40, 41};
	// // v = {3, 7, 12, 18, 25, 31, 39, 42, 50, 57, 63, 69, 75, 81, 88, 94, 99, 105, 112, 118, 124, 130, 137, 143, 150, 157, 163, 170, 176, 183};
	// // v = {144, 127, 179, 160, 150, 187, 167};	
	// // v = {144, 179, 127, 135, 100, 130};
	// id = 0;
}

void World::setMode(World::Mode mode){
// <<<<<<< HEAD
// 	ModeContainer->ChangeActivateChild(mode); // ensure that the Modecontainer handle the true index of activated child
// 	OperationButtonsList->makeEmpty();
// 	if (mode == World::Mode::AVL)
// 	{

// 		/* FUNCTIONAL BUTTONS */
// 		sf::Vector2f insertButtonPos(20.f, mWindow.getSize().y - GUI::ButtonSize.y - 20.f); 
// 		GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), insertButtonPos, "Insert");
// 		sf::Vector2f inputBoxInsertPos(insertButtonPos.x + 150 + 10.f, insertButtonPos.y);
// 		GUI::DeliverTextBox::Ptr InputBoxInsert = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main),inputBoxInsertPos,sf::Vector2f(100.f, 40.f));
// 		InputBoxInsert->setButtonParent(InsertButton);
// 		InsertButton->setCallback([this,InsertButton](){
// 			if(InsertButton->getSubComponentInfo().num != -1) {
// 				this->mDataStructure->insert(InsertButton->getSubComponentInfo().num);
// 				InsertButton->setSubComponentInfo(-1);
// 				// std::cout<<"Insert ok";
// 			}
// 			else if(InsertButton->getSubComponentInfo().VecNum.size() != 0){
// 				for(auto& element: InsertButton->getSubComponentInfo().VecNum){
// 					this->mDataStructure->insert(element);
// 				}
// 			}
// 		});

// 		sf::Vector2f deleteButtonPos(20.f, insertButtonPos.y - GUI::ButtonSize.y - 10.f);
// 		GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), deleteButtonPos, "Delete");
// 		sf::Vector2f inputBoxDeletePos(deleteButtonPos.x + 150 + 10.f, deleteButtonPos.y);
// 		GUI::DeliverTextBox::Ptr InputBoxDelete = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main), inputBoxDeletePos, sf::Vector2f(100.f, 40.f));
// 		InputBoxDelete->setButtonParent(DeleteButton);
// 		DeleteButton->setCallback([this,DeleteButton](){
// 			if(DeleteButton->getSubComponentInfo().num != -1){
// 				this->mDataStructure->remove(DeleteButton->getSubComponentInfo().num);
// 				DeleteButton->setSubComponentInfo(-1);
// 				// std::cout<<"Delete num"<<std::endl;
// 			}
// 			else if(DeleteButton->getSubComponentInfo().VecNum.size() != 0){
// 				for(auto& element: DeleteButton->getSubComponentInfo().VecNum){
// 					this->mDataStructure->remove(element);
// 					// std::cout<<"Delete list of num"<<std::endl;
// 				}
// 			}
// 		});
		
// 		// Add search Button
// 		InsertButton->addSubComponent(InputBoxInsert);
// 		DeleteButton->addSubComponent(InputBoxDelete);
// 		OperationButtonsList->pack(InsertButton);
// 		OperationButtonsList->pack(DeleteButton);
// =======
	//ModeContainer->ChangeActivateChild(mode); // ensure that the Modecontainer handle the true index of activated child
	// delete mDataStructure;
	// mDataStructure = new LinkedList();

	// if(mDataStructure){
	// 	delete mDataStructure;
	// }

	if(mode == World::Mode::AVLMode){
		std::unique_ptr<AVLTree> root(new AVLTree());
		mDataStructure = root.get();
		mSceneLayers[DataStructure]->attachChild(std::move(root));
		mSceneLayers[DataStructure]->setPosition(sf::Vector2f(1000, 100));
	}
	else if(mode == World::Mode::LinkedListMode){
		std::unique_ptr<LinkedList> root(new LinkedList());
		mDataStructure = root.get();
		mSceneLayers[DataStructure]->attachChild(std::move(root));
		mSceneLayers[DataStructure]->setPosition(sf::Vector2f(1000, 100));
	}
	else if(mode == World::Mode::HeapMode){
		// mDataStructure = new Heap();
	}
	else if(mode == World::Mode::GraphMode){
		// mDataStructure = new Graph();
	}

	
	mMainUI->CreateButtonList(mode,mDataStructure);
	// OperationButtonsList->makeEmpty();
	// if(mode == World::Mode::AVL)
	// {

	// 	// std::unique_ptr<AVLTree> root(new AVLTree());
	// 	// mDataStructure = root.get();
	// 	// mDataStructure->setPosition(sf::Vector2f(600, 100));
	// 	// mSceneLayers[DataStructure]->attachChild(std::move(root));

	// 	/* FUNCTIONAL BUTTONS */
	// 	sf::Vector2f insertButtonPos(20.f, mWindow.getSize().y - GUI::ButtonSize.y - 20.f); 
	// 	GUI::ExpandableButton::Ptr InsertButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), insertButtonPos, "Insert");
	// 	sf::Vector2f inputBoxInsertPos(insertButtonPos.x + 150 + 10.f, insertButtonPos.y);
	// 	GUI::DeliverTextBox::Ptr InputBoxInsert = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main),inputBoxInsertPos,sf::Vector2f(100.f, 40.f));
	// 	InputBoxInsert->setButtonParent(InsertButton);
	// 	InsertButton->setCallback([this,InsertButton](){
	// 		if(InsertButton->getSubComponentInfo().num != -1) {
	// 			this->mDataStructure->insert(InsertButton->getSubComponentInfo().num);
	// 			InsertButton->setSubComponentInfo(-1);
	// 			std::cout<<"Insert ok";
	// 		}
	// 		else if(InsertButton->getSubComponentInfo().VecNum.size() != 0){
	// 			for(auto& element: InsertButton->getSubComponentInfo().VecNum){
	// 				this->mDataStructure->insert(element);
	// 			}
	// 		}
	// 	});

	// 	sf::Vector2f deleteButtonPos(20.f, insertButtonPos.y - GUI::ButtonSize.y - 10.f);
	// 	GUI::ExpandableButton::Ptr DeleteButton = std::make_shared<GUI::ExpandableButton>(mFont.get(Fonts::ID::Main), deleteButtonPos, "Delete");
	// 	sf::Vector2f inputBoxDeletePos(deleteButtonPos.x + 150 + 10.f, deleteButtonPos.y);
	// 	GUI::DeliverTextBox::Ptr InputBoxDelete = std::make_shared<GUI::DeliverTextBox>(mFont.get(Fonts::ID::Main), inputBoxDeletePos, sf::Vector2f(100.f, 40.f));
	// 	InputBoxDelete->setButtonParent(DeleteButton);
	// 	DeleteButton->setCallback([this,DeleteButton](){
	// 		if(DeleteButton->getSubComponentInfo().num != -1){
	// 			this->mDataStructure->remove(DeleteButton->getSubComponentInfo().num);
	// 			DeleteButton->setSubComponentInfo(-1);
	// 			std::cout<<"Delete num"<<std::endl;
	// 		}
	// 		else if(DeleteButton->getSubComponentInfo().VecNum.size() != 0){
	// 			for(auto& element: DeleteButton->getSubComponentInfo().VecNum){
	// 				this->mDataStructure->remove(element);
	// 				std::cout<<"Delete list of num"<<std::endl;
	// 			}
	// 		}
	// 	});
		
	// 	// Add search Button
	// 	InsertButton->addSubComponent(InputBoxInsert);
	// 	DeleteButton->addSubComponent(InputBoxDelete);
	// 	OperationButtonsList->pack(InsertButton);
	// 	OperationButtonsList->pack(DeleteButton);
	// }
	// 3 other data structures
}

void World::handleEvent(const sf::Event& event){

	ModeContainer->handleEvent(event);
	mMainUI->handleEvent(event);


// <<<<<<< HEAD
// 		if (event.key.code == sf::Keyboard::U)
// 			mDataStructure->empty();
// =======
// 	updateBackRequest();
// >>>>>>> GUI

	// if (event.type == sf::Event::KeyPressed)
    // {
    //     if (event.key.code == sf::Keyboard::A)
    //     {
    //         // int value = std::rand() % 100;
    //         int value = v[id];
    //         id++;
    //         // std::cerr << value << "\n";
    //         mDataStructure->insert(value);
    //     }

	// 	if (event.key.code == sf::Keyboard::U)
	// 		mDataStructure->undo();
    //     // if (event.key.code == sf::Keyboard::L)
    //     //     mDataStructure->leftRotate();

	// 	// if (event.key.code == sf::Keyboard::R)
    //     //     mDataStructure->rightRotate();

    //     if (event.key.code == sf::Keyboard::D)
    //     {
    //         if (mDataStructure->search(5))
    //             std::cerr << "Found \n";
    //         else 
    //             std::cerr << "Cannot find \n";
    //     }
    // }
}

void World::updateBackRequest()
{
	BackRequest = mMainUI->getBackRequest();
}

bool World::getBackRequest(){
	return BackRequest;
}
