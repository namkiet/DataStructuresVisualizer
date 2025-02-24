#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode.hpp>
#include <ResourceHolder.hpp>
#include <ResourceIdentifiers.hpp>

class CircleNode : public SceneNode
{
public:
                        CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

private:
    sf::CircleShape     shape;
    sf::Text            text;

private:
    virtual void        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};