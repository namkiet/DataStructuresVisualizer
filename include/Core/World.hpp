#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <DataStructures/AVLTree.hpp>
#include <DataStructures/HeapTree.hpp>
#include <SceneNode/SpriteNode.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>
#include <array>
#include <vector>
#include <queue>
#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/ExpandableButton.hpp>
#include <GUI/DeliverTextBox.hpp>

#include <SceneNode/PseudoCode.hpp>

class World : private sf::NonCopyable
{
private:
    std::vector<int> v;
    int id;

public:
    explicit							World(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);
    void								update(sf::Time dt);
    void								draw();

private:
    sf::RenderWindow&					mWindow;
    TextureHolder						mTextures;

public:
    enum Mode{
        AVL,
        LinkedList,
        Heap,
        Graph,
        None
    };

private:
    void								loadTextures();
    void								buildScene();
    // void                                initPseudoCode();
    void                                CreateModeContainer();

private:
    enum Layer
    {
        DataStructure,
        Background,
        CodeBox,
        Controller,
        OperationButtons,
        LayerCount
    };

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;

    DS*                                 mDataStructure;
    PseudoCode*                         mPseudoCode;

public:
    CommandQueue&						getCommandQueue();
    void                                handleEvent(const sf::Event& event);
    void                                setMode(World::Mode mode);
    
private:
    CommandQueue						mCommandQueue;
    FontHolder&                         mFont;

private:
    sf::View							mWorldView;
    GUI::Container::Ptr                 OperationButtonsList;
    Mode                                mMode;
    GUI::Container::Ptr                 ModeContainer;

};