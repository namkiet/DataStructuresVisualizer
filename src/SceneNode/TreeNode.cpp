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
    mHeight(1),
    mColor(fillColor),
    mLeftEdge(outlineColor, sf::Vector2f(0, 0), sf::Vector2f(0, 0)),
    mRightEdge(outlineColor, sf::Vector2f(0, 0), sf::Vector2f(0, 0))
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

    // if (mLeft) mLeft->update(dt);
    // if (mRight) mRight->update(dt);
    
    // Update left edge
    if (mLeft) // If current node has left child
    {
        // sf::Vector2f dir = mLeft->getPosition() - getPosition();
        // float radius = mShape.getRadius() + mShape.getOutlineThickness();
        // if (norm(dir) >= radius * 2)
        // {
        //     sf::Vector2f offset = dir * (radius / norm(dir));
        //     mLeftEdge.setHead(offset);
        //     mLeftEdge.setTail(dir - offset);
        // }
        mLeft->update(dt);
    }
    // else
    // {
    //     mLeftEdge.setHead(sf::Vector2f(0, 0));
    //     mLeftEdge.setTail(sf::Vector2f(0, 0));
    // }

    // Update right edge
    if (mRight) // If current node has right child
    {
        // sf::Vector2f dir = mRight->getPosition() - getPosition();
        // float radius = mShape.getRadius();
        // if (norm(dir) >= radius * 2)
        // {
        //     sf::Vector2f offset = dir * (radius / norm(dir));
        //     mRightEdge.setHead(offset);
        //     mRightEdge.setTail(dir - offset);
        // }
        mRight->update(dt);
    }
    // else
    // {
    //     mRightEdge.setHead(sf::Vector2f(0, 0));
    //     mRightEdge.setTail(sf::Vector2f(0, 0));
    // }
}


void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform = getTransform();
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
    target.draw(mLeftEdge, states);
    target.draw(mRightEdge, states);
}

sf::Color TreeNode::getColor()
{
    return mShape.getFillColor();
}

void TreeNode::setColor(sf::Color color)
{
    mShape.setFillColor(color);
}

void TreeNode::setOpacity(float opacity)
{
    if (opacity > 1) return;
    sf::Color newFillColor = mShape.getFillColor();
    newFillColor.a = 255 * opacity;
    sf::Color newOutlineColor = mShape.getOutlineColor();
    newOutlineColor.a = 255 * opacity;
    sf::Color newTextColor = mText.getFillColor();
    newTextColor.a = 255 * opacity;
    
    mShape.setFillColor(newFillColor);
    mShape.setOutlineColor(newOutlineColor);
    mText.setFillColor(newTextColor);
}