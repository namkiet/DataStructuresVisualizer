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

namespace GUI
{
    class MainUI
    {   
        public:
            typedef std::shared_ptr<MainUI>     Ptr;
        private:
            sf::Font                            mFont;

            sf::RectangleShape                  toolbox;
            sf::RectangleShape                  Background;
            std::vector<sf::VertexArray>        mSeperateToolBoxLine;
            sf::RectangleShape                  OperationBox;
            std::vector<sf::Vector2f>           OperationButtonPosition;
            bool                                BackRequest;

            GUI::Container::Ptr                 OperationButtonsList;
            GUI::Button::Ptr                    BackButtons;

            // GUI::Container::Ptr                 ModeContainer;


        public:
            MainUI(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);
            void			                    draw(sf::RenderTarget& target, sf::RenderStates states) const;
            void                                CreateButtonList(World::Mode mode, DS* mDataStructure);
            void                                handleEvent(const sf::Event& event);
            bool                                getBackRequest();
    };
    



}
