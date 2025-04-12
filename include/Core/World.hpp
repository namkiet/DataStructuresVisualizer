#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <DataStructures/AVLTree.hpp>
#include <DataStructures/HeapTree.hpp>
#include <array>
#include <vector>
#include <queue>
#include <GUI/Button.hpp>
#include <GUI/Container.hpp>
#include <GUI/ExpandableButton.hpp>
#include <GUI/DeliverTextBox.hpp>

#include <DataStructures/LinkedList.hpp>

#include <SceneNode/PseudoCode.hpp>
#include <SceneNode/InfoPanel.hpp>

class MainUI;

class World : private sf::NonCopyable
{
private:
    sf::Sprite background;
    
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
    void								buildScene();
    void                                updateBackRequest();

private:
    enum Layer
    {
        Background,
        SidePanel,
        CodeBox,
        InfoBox,
        DataStructure,
        LayerCount
    };

    SceneNode							mSceneGraph;
    std::array<SceneNode*, LayerCount>	mSceneLayers;

    DS*                                 mDataStructure;
    PseudoCode*                         mPseudoCode;
    InfoPanel*                          mInfoPanel;
    MainUI*                             mMainUI;

public:
    void                                handleEvent(const sf::Event& event);
    void                                setMode(World::Mode mode);
    bool                                getBackRequest();
    
private:
    sf::View							mWorldView;
    Mode                                mMode;
    bool                                BackRequest;

    sf::Shader                          mShader;

};