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

    v = {135, 67, 271, 187, 312, 210, 124, 87, 33, 48, 72, 83, 54, 235, 129};
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

        if (event.key.code == sf::Keyboard::C)
            mAVL.leftRotate();

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
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer = std::make_unique<SceneNode>();
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

    mAVL.setPosition(sf::Vector2f(600, 100));
}