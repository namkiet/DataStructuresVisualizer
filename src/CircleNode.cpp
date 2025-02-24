#include <CircleNode.hpp>
#include <Utility.hpp>

CircleNode::CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor)
{  
    shape.setRadius(radius);
    shape.setOrigin(sf::Vector2f(radius, radius));
    shape.setFillColor(fillColor);
    shape.setOutlineColor(outlineColor);
    shape.setOutlineThickness(2);
    shape.setPointCount(1000);


    // text.setColor(outlineColor);
    // text.setCharacterSize(radius * 0.75);
    // text.setFont(font);
    // text.setString(std::to_string(value));
    // centerOrigin(text);
    // text.setPosition(shape.getPosition());
}

void CircleNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(shape, states);
    // target.draw(text, states);
}