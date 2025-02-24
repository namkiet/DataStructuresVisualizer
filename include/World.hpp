#ifndef BOOK_WORLD_HPP
#define BOOK_WORLD_HPP

#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>
#include <SpriteNode.hpp>
#include <CircleNode.hpp>
// #include <Aircraft.hpp>
// #include <CommandQueue.hpp>
// #include <Command.hpp>

#include <SFML/Graphics.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
    explicit							World(sf::RenderWindow& window, TextureHolder& textures);
    void								update(sf::Time dt);
    void								draw();
    // CommandQueue&						getCommandQueue();

private:
    void								loadTextures();
    void								buildScene();

private:
    enum Layer
    {
        Background,
        Objects,
        CodeBox,
        Controller,
        OperationButotns,
        LayerCount
    };


private:
    sf::RenderWindow&					mWindow;
    sf::View							mWorldView;
    TextureHolder						mTextures;

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;
    // CommandQueue						mCommandQueue;

    sf::FloatRect						mWorldBounds;
    // sf::Vector2f						mSpawnPosition;
    // float								mScrollSpeed;
    // Aircraft*							mPlayerAircraft;
    CircleNode*                         mRootNode;
};

#endif // BOOK_WORLD_HPP
