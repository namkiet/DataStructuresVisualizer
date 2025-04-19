#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <State/State.hpp>
#include <GUI/Container.hpp>
#include <GUI/Button.hpp>
#include <GUI/ExpandableButton.hpp>
#include <Core/World.hpp>
#include "DataStructures/DS.hpp"
#include<string>

class MainUI: public SceneNode
{   
public:
    typedef std::unique_ptr<MainUI>     Ptr;

private:
    sf::Font                            mFont;

private:
    sf::RectangleShape                  ToolBox;
    std::vector<sf::VertexArray>        mSeperateToolBoxLine;
    sf::RectangleShape                  OperationBox;
    std::vector<sf::Vector2f>           OperationButtonPosition;
    bool                                BackRequest;

    GUI::Container::Ptr                 OperationButtonsList;
    GUI::Button::Ptr                    BackButtons;

    std::vector<float>                  speed = {0.25f, 0.5f, 1.f, 1.5f, 2.f};
    int                                 speedIndex = 2;
    GUI::Button::Ptr                    mSpeedButton;

    sf::Sprite                          mSelectedRadioButtonSprite;
    sf::Sprite                          mUnselectedRadioButtonSprite;
    GUI::Button::Ptr                    mStepByStepButton;
    GUI::Button::Ptr                    mAtOnceButton;


public:
                                        MainUI(TextureHolder& textures, FontHolder& fonts);
    virtual void                        updateCurrent(sf::Time dt);
    virtual void                        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void                                handleEvent(const sf::Event& event);

public:
    void                                createButtonList(World::Mode mode, DS* mDataStructure, InfoPanel *info);
    void                                initAVLButtons(AVLTree* avl, InfoPanel *info);
    void                                initHeapButtons(HeapTree* heap, InfoPanel *info);
    void                                initLinkedListButtons(LinkedList* ll, InfoPanel *info);
    void                                initGraphButtons(Graph* g, InfoPanel *info);
    bool                                getBackRequest();
};
