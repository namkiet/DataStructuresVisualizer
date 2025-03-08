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

    v = {6,1, 3, 2, 10, 4, 5, 6, 7, 8};
    id = 0;
}

void AVL::draw()
{
	// mWindow.draw(mSceneGraph);   
    mWindow.draw(mAVL);
}

bool AVL::update(sf::Time dt)
{
	// while (!mCommandQueue.isEmpty())
    //     mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
    // mSceneGraph.update(dt);
    mAVL.update(dt);
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
            mAVL.insert(value);
        }

        if (event.key.code == sf::Keyboard::B)
            mAVL.setPosition(mAVL.getPosition() + sf::Vector2f(50, 50));

        // if (event.key.code == sf::Keyboard::C)
            // mAVL.leftRotate();

        if (event.key.code == sf::Keyboard::D)
        {
            std::cerr << mAVL.search(5) << "\n";
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
    std::cerr << LayerCount << "\n";
	// Initialize the different layers
	// for (std::size_t i = 0; i < LayerCount; ++i)
	// {
	// 	SceneNode::Ptr layer = std::make_unique<SceneNode>();
	// 	mSceneLayers[i] = layer.get();
	// 	mSceneGraph.attachChild(std::move(layer));
	// }

	// // Add first object
	// auto firstObj = std::make_unique<TreeNode>(20, 20.f, sf::Color::White, sf::Color::Black);
	// firstObj->setPosition(600, 100);
	// mRootNode = firstObj.get();
	// mSceneLayers[Nodes]->attachChild(std::move(firstObj));

    mAVL.setPosition(sf::Vector2f(600, 100));
}