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

class MainUI: public SceneNode
{   
    public:
        typedef std::unique_ptr<MainUI>     Ptr;
    private:
        sf::Font                            mFont;

        sf::RectangleShape                  ToolBox;
        sf::RectangleShape                  Background;
        std::vector<sf::VertexArray>        mSeperateToolBoxLine;
        sf::RectangleShape                  OperationBox;
        std::vector<sf::Vector2f>           OperationButtonPosition;
        bool                                BackRequest;

        GUI::Container::Ptr                 OperationButtonsList;
        GUI::Button::Ptr                    BackButtons;

        // GUI::Container::Ptr                 ModeContainer;


    public:
                                            MainUI(TextureHolder& textures, FontHolder& fonts);
        virtual void                        updateCurrent(sf::Time dt);
        virtual void                        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void                                handleEvent(const sf::Event& event);

    public:
        void                                createButtonList(World::Mode mode, DS* mDataStructure);
        void                                initAVLButtons(AVLTree* avl);
        void                                initHeapButtons(HeapTree* avl);
        void                                initLinkedListButtons(LinkedList* ll);
        // void                                initGraphButtons(Graph* graph);
        bool                                getBackRequest();
};
