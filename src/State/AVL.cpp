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
}

void AVL::draw()
{
	mWindow.draw(mSceneGraph);   
    mWindow.draw(mAVL);
}

bool AVL::update(sf::Time dt)
{
	while (!mCommandQueue.isEmpty())
        mSceneGraph.executeCommand(mCommandQueue.pop(), dt);
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
            int value = std::rand() % 50;
            std::cerr << value << "\n";

            mAVL.insert(value);

            
            // TreeNode* cur = mRootNode;
            // TreeNode* prev = nullptr;

            // while (cur)
            // {
            //     prev = cur;
            //     if (value < cur->getValue()) cur = cur->getLeft();
            //     else if (value > cur->getValue()) cur = cur->getRight();
            //     else return true;
            // }
            // cur = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
            // // cur->setPosition(sf::Vector2f(50, 50));
            // if (prev->getValue() < value)
            //     prev->attachRight(cur);
            // else
            //     prev->attachLeft(cur);
        }

        // if (event.key.code == sf::Keyboard::B)
        //     mRootNode->moveTo(sf::Vector2f(100, 200));
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
	// mSceneLayers[Nodes]->attachChild(std::move(firstObj));

    // mSceneLayers[Nodes]->attachChild()
    mAVL.setPosition(sf::Vector2f(600, 100));
}

void AVL::insert(int value)
{
}