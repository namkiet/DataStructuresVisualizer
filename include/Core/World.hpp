#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/SpriteNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>
#include <array>
#include<vector>
#include <queue>
#include "Core/Button.hpp"
#include "Core/Container.hpp"
class World : private sf::NonCopyable
{
private:
    struct Node
    {
        int value;
        Node *left, *right, *parent;
        Node(int value): value(value), left(nullptr), right(nullptr) {};
    };

public:
    explicit							World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);
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
        OperationButtons,
        LayerCount
    };

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;
    TreeNode*                           mRootNode;

public:
    CommandQueue&						getCommandQueue();
    void                                handleEvent(const sf::Event& event);

private:
    CommandQueue						mCommandQueue;

private:
    // sf::FloatRect						mWorldBounds; 
    sf::View							mWorldView;
    Container                           ButtonList;
};