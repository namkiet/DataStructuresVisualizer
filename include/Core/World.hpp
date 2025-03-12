#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <DataStructures/AVLTree.hpp>
#include <SceneNode/SpriteNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>
#include <array>
#include<vector>
#include <queue>
#include "GUI/Button.hpp"
#include "GUI/Container.hpp"
#include "GUI/ExpandableButton.hpp"
#include "GUI/TextBox.hpp"
class World : private sf::NonCopyable
{
private:
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
        Tree,
        Background,
        Objects,
        CodeBox,
        Controller,
        OperationButtons,
        LayerCount
    };

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;
    // SceneNode::Ptr                      mRootNode;
    AVLTree*    mAVL; 

public:
    enum class Mode{
        LinkedList,
        AVL,
        Heap,
        Graph,
        None
    };
    void                                CreateModeContainer();

    CommandQueue&						getCommandQueue();
    void                                handleEvent(const sf::Event& event);
    void                                setMode(World::Mode mode);
private:
    CommandQueue						mCommandQueue;
    FontHolder&                         mFont;

private:
    sf::View							mWorldView;
    std::vector<GUI::ExpandableButton::Ptr>         OperationButtonsList;
    Mode                                mMode;
    GUI::Container::Ptr                 ModeContainer;

};