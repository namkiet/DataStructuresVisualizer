#pragma once
#include <SFML/Graphics.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>
#include <SceneNode.hpp>
#include <SpriteNode.hpp>
#include <CircleNode.hpp>
// #include <Aircraft.hpp>
// #include <CommandQueue.hpp>
// #include <Command.hpp>
#include <array>
#include <queue>

class World : private sf::NonCopyable
{
public:
    explicit							World(sf::RenderWindow& window, TextureHolder& textures);
    void								update(sf::Time dt);
    void								draw();

private:
    sf::RenderWindow&					mWindow;
    TextureHolder						mTextures;

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

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;
    CircleNode*                         mRootNode;

public:
    CommandQueue&						getCommandQueue();

private:
    CommandQueue						mCommandQueue;

private:
    // sf::FloatRect						mWorldBounds; 
    sf::View							mWorldView;
};