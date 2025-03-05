#include <SceneNode/TreeNode.hpp>
#include <Core/Utility.hpp>
#include <Core/Animation.hpp>
#include <memory>
#include <iostream>

TreeNode::TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    mValue(value),
    mLeft(nullptr),
    mRight(nullptr),
    mParent(nullptr),
    mColor(fillColor)
{  
    mShape.setRadius(radius);
    mShape.setOrigin(sf::Vector2f(radius, radius));
    mShape.setFillColor(fillColor);
    mShape.setOutlineColor(outlineColor);
    mShape.setOutlineThickness(2);
    mShape.setPointCount(10000);

    font.loadFromFile("assets/fonts/jetbrains.ttf");
    mText.setColor(sf::Color::Black);
    mText.setCharacterSize(radius * 0.75);
    mText.setFont(font);
    mText.setString(std::to_string(value));
    centerOrigin(mText);
    mText.setPosition(mShape.getPosition());
}

void TreeNode::update(sf::Time dt)
{
    updateCurrent(dt);
    if (mLeft) mLeft->update(dt);
    if (mRight) mRight->update(dt);
}


void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    if (mLeft) mLeft->draw(target, states);
    if (mRight) mRight->draw(target, states);    
}

void TreeNode::updateCurrent(sf::Time dt)
{
    
}

void TreeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape, states);
    target.draw(mText, states);
}

sf::Color TreeNode::getColor()
{
    return mShape.getFillColor();
}

void TreeNode::setColor(sf::Color color)
{
    mShape.setFillColor(color);
}