#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>

#include <SceneNode/SceneNode.hpp>
#include <SceneNode/PseudoCode.hpp>
#include <DataStructures/DS.hpp>
#include <DataStructures/AVLTree.hpp>
#include <DataStructures/HeapTree.hpp>
#include <DataStructures/LinkedList.hpp>
#include <GUI/InfoPanel.hpp>
#include <GUI/MainUI.hpp>

#include <array>
#include <vector>
#include <queue>

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
    void                                CreateModeContainer();
    void                                updateBackRequest();


private:
    enum Layer
    {
        Background,
        DataStructure,
        CodeBox,
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
    void                                handleEvent(const sf::Event& event);
    void                                setMode(World::Mode mode);
    bool                                getBackRequest();

private:
    sf::View							mWorldView;
    Mode                                mMode;
    bool                                BackRequest;
};