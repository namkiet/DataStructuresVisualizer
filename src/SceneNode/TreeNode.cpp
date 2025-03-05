#include <SceneNode/TreeNode.hpp>
#include <Core/Utility.hpp>
#include <memory>
#include <iostream>

TreeNode::TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    mIsMoving(false),
    mIsHighlighted(false),
    mIsAnimationFinished(true),
    mValue(value)
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

// void TreeNode::update(sf::Time dt)
// {
//     updateCurrent(dt);
//     // if (mIsAnimationFinished)
//     // {
//         if (mLeft) mLeft->update(dt);
//         if (mRight) mRight->update(dt);
//     // }
// }


// void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     states.transform *= getTransform();
//     drawCurrent(target, states);
//     if (mLeft) mLeft->draw(target, states);
//     if (mRight) mRight->draw(target, states);    
// }

void TreeNode::updateCurrent(sf::Time dt)
{
    mIsAnimationFinished = true;
    if (mIsMoving)
    {
        mIsAnimationFinished = false;
        sf::Vector2f curPosition = getPosition();
        if (dist(curPosition, mTargetPosition) < 1.f)
        {
            setPosition(mTargetPosition);
            mIsMoving = false;
        }
        else
        {
            sf::Vector2f dir = mTargetPosition - curPosition;
            sf::Vector2f unitDir = dir * (1 / dist(curPosition, mTargetPosition));
            move(unitDir * 400.f * dt.asSeconds());
        }
    }

    if (mIsHighlighted)
    {
        mIsAnimationFinished = false;
        if (mElapsedTime < mDuration)
        {
            if (mElapsedTime >= 0) mShape.setFillColor(sf::Color::Yellow);
            mElapsedTime += dt.asSeconds();
        }
        else
        {
            mShape.setFillColor(sf::Color::White);
            mElapsedTime = 0;
            mDuration = 0;
            mIsHighlighted = 0;
        }
        
    }
}

void TreeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mShape, states);
    target.draw(mText, states);
}

void TreeNode::moveTo(sf::Vector2f target)
{
    mIsMoving = true;
    mTargetPosition = target;
}

void TreeNode::highlight(sf::Color targetColor, float elapsedTime, float duration)
{
    mIsHighlighted = true;
    mTargetColor = targetColor;
    mElapsedTime = 0.f;
    mDuration = 0.2f;
}

int TreeNode::getValue()
{
    return mValue;
}

void TreeNode::setValue(int value)
{
    mValue = value;
}