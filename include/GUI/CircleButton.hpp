#pragma once
#include <GUI/Button.hpp>

namespace GUI
{

class CircleButton: public Button
{    
    public:
                                CircleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Texture& texture);
                                CircleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font& font, std::string text);
    
    public:
        void                    setSize(sf::Vector2f size) override;
};

}