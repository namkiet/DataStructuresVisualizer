#include <GUI/CircleButton.hpp>

GUI::CircleButton::CircleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Texture &texture):
    Button(position, buttonSize, texture)
{
    mShape = new sf::CircleShape(buttonSize.x);
    static_cast<sf::CircleShape*>(mShape)->setOrigin(buttonSize * 0.5);
}

GUI::CircleButton::CircleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font& font, std::string text):
    Button(position, buttonSize, font, text)
{   
    mShape = new sf::CircleShape(buttonSize.x);
    static_cast<sf::CircleShape*>(mShape)->setOrigin(buttonSize * 0.5);
}

void GUI::CircleButton::setSize(sf::Vector2f size)
{
    GUI::Button::setSize(size);
    static_cast<sf::CircleShape*>(mShape)->setOrigin(size * 0.5);
    static_cast<sf::CircleShape*>(mShape)->setRadius(size.x);
}