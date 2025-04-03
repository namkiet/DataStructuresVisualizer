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

#include <GUI/InfoPanel.hpp>
#include <DataStructures/LinkedList.hpp>

#include <SceneNode/PseudoCode.hpp>

class MainUI;

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
    TextureHolder&					    mTextures;
    FontHolder&                         mFonts;

public:
    enum Mode {
        AVLMode,
        LinkedListMode,
        HeapMode,
        GraphMode,
        NoneMode
    };

private:
    void								loadTextures();
    void								buildScene();
    // void                                initPseudoCode();
    void                                CreateModeContainer();
    void                                updateBackRequest();


private:
    enum Layer
    {
        Background,
        DataStructure,
        SidePanel,
        LayerCount
    };

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;

    DS*                                 mDataStructure;
    PseudoCode*                         mPseudoCode;
    MainUI*                             mMainUI;
    GUI::InfoPanel*                     mInfoPanel;

public:
    CommandQueue&						getCommandQueue();
    void                                handleEvent(const sf::Event& event);
    void                                setMode(World::Mode mode);
    bool                                getBackRequest();
    
private:
    CommandQueue						mCommandQueue;

private:
    sf::View							mWorldView;
    Mode                                mMode;
    bool                                BackRequest;



};