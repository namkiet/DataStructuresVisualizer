#include <State/AVL.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>
#include <cstdlib>
#include <random>
#include <iostream>

AVL::AVL(StateStack& stack, Context context): 
    State(stack, context),
    mWindow(*context.window),
    mTextures(*context.textures)
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	text.setFont(font);
	text.setString("This is AVL");
	centerOrigin(text);
	text.setPosition(context.window->getView().getSize() / 2.f);

    loadTextures();
    buildScene();

    // v = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 85};
    // v = {6, 5, 3, 4, 1,2 ,8};
    // v = {10,20,30,40,50,60,70,80,90,100,110};
    v = {150, 70, 92, 186, 215, 83, 74, 90, 88, 46, 79, 137, 77, 63, 191};
    id = 0;
}

void AVL::draw()
{
	mWindow.draw(mSceneGraph);   
    // mWindow.draw(mAVL);
}

bool AVL::update(sf::Time dt)
{
	// while (!mCommandQueue.isEmpty())
    //     mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
    mSceneGraph.update(dt);
    // mAVL->update(dt);
	return true;
}

bool AVL::handleEvent(const sf::Event& event)
{
    // CommandQueue& commands = mWorld.getCommandQueue();

    // CommandQueue& commands = getCommandQueue();
    // mUser.handleEvent(event, commands);

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {   
            requestStackPop();
            requestStackPush(States::Menu);
        }

        if (event.key.code == sf::Keyboard::A)
        {
            // int value = std::rand() % 100;
            int value = v[id];
            id++;
            std::cerr << value << "\n";
            mAVL->insert(value);
        }

        if (event.key.code == sf::Keyboard::B)
            mAVL->setPosition(mAVL->getPosition() - sf::Vector2f(50, 0));

        if (event.key.code == sf::Keyboard::L)
            mAVL->leftRotate();

        if (event.key.code == sf::Keyboard::R)
            mAVL->rightRotate();

        if (event.key.code == sf::Keyboard::D)
        {
            if (mAVL->search(5))
                std::cerr << "Found \n";
            else 
                std::cerr << "Cannot find \n";
        }
    }

	return true;
}


CommandQueue& AVL::getCommandQueue()
{
	return mCommandQueue;
}

void AVL::loadTextures()
{
	// mTextures.load(Textures::AppBackground, "assets/images/background.jpg");
}

void AVL::buildScene()
{
    // std::cerr << LayerCount << "\n";
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer = std::make_unique<SceneNode>();
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

    std::unique_ptr<AVLTree> root(new AVLTree());
    mAVL = root.get();
    mAVL->setPosition(sf::Vector2f(600, 100));
    mSceneLayers[Tree]->attachChild(std::move(root));
}