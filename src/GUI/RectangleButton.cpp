#include <GUI/RectangleButton.hpp>

GUI::RectangleButton::RectangleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Texture &texture):
    Button(position, buttonSize, texture)
{
    mShape = new sf::RectangleShape(buttonSize);
}

GUI::RectangleButton::RectangleButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font& font, std::string text):
    Button(position, buttonSize, font, text)
{   
    mShape = new sf::RectangleShape(buttonSize);
}

void GUI::RectangleButton::setSize(sf::Vector2f size)
{
    GUI::Button::setSize(size);
    static_cast<sf::RectangleShape*>(mShape)->setSize(size);
}