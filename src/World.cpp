#include <SFML/Graphics/RenderWindow.hpp>
#include <World.hpp>
#include <algorithm>
#include <cmath>

World::World(sf::RenderWindow& window, TextureHolder& textures):
    mWindow(window),
    mWorldView(mWindow.getDefaultView()),
    mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y),
	mRootNode(nullptr)
{
	loadTextures();
	buildScene();
}

void World::update(sf::Time dt)
{
	// Scroll the world, reset player velocity
	// mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
	// mPlayerAircraft->setVelocity(0.f, 0.f);

	// Forward commands to scene graph, adapt velocity (scrolling, diagonal correction)
	// while (!mCommandQueue.isEmpty())
	// 	mSceneGraph.onCommand(mCommandQueue.pop(), dt);
	// adaptPlayerVelocity();

	// Regular update step, adapt position (correct if outside view)
	// adaptPlayerPosition();

	mSceneLayers[Objects]->move(sf::Vector2f(0, 100.f) * dt.asSeconds());

	mSceneGraph.update(dt);
}

void World::draw()
{
	mWindow.draw(mSceneGraph);
}

// CommandQueue &World::getCommandQueue()
// {
// 	return mCommandQueue;
// }

void World::loadTextures()
{
	mTextures.load(Textures::AppBackground, "assets/images/background.jpg");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer = std::make_unique<SceneNode>();
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	// Add the background sprite to the scene
	sf::Texture &texture = mTextures.get(Textures::AppBackground);
	sf::IntRect textureRect(mWorldBounds);

	std::unique_ptr<SpriteNode> backgroundSprite = std::make_unique<SpriteNode>(texture, textureRect);
	backgroundSprite->setPosition(0, 0);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add first object
	std::unique_ptr<CircleNode> firstObj = std::make_unique<CircleNode>(20, 20.f, sf::Color::White, sf::Color::Black);
	firstObj->setPosition(600, 100);
	mRootNode = firstObj.get();
	mSceneLayers[Objects]->attachChild(std::move(firstObj));

	// Add second object
	std::unique_ptr<CircleNode> secondObj = std::make_unique<CircleNode>(10, 20.f, sf::Color::White, sf::Color::Black);
	secondObj->setPosition(-100, 100);
	mRootNode->attachChild(std::move(secondObj));

	// Add third object
	std::unique_ptr<CircleNode> thirdObj = std::make_unique<CircleNode>(-5, 20.f, sf::Color::White, sf::Color::Black);
	thirdObj->setPosition(100, 100);
	mRootNode->attachChild(std::move(thirdObj));
}