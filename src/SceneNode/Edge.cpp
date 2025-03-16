#include <SceneNode/Edge.hpp>
#include <Core/Utility.hpp>
#include<iostream>

Edge::Edge(sf::Color color, TreeNode* from, TreeNode* to, bool hasArrow): 
    mFrom(from),
    mTo(to),
    mLine(sf::PrimitiveType::LineStrip, 2),
    mColor(color),
    mHasArrow(hasArrow),
    mIsChangingTail(false)
{   
    mArrowSize = 12;
    mArrowHead.setPointCount(3);

    mHead = mFrom->getPosition();
    mTail = (mTo ? mTo->getPosition() : mFrom->getPosition());

    buildEdge();
}


void Edge::buildEdge()
{
    mLine[0].color = mColor;
    mLine[1].color = mColor;
    mArrowHead.setFillColor(mColor);

    sf::Vector2f dir = mTail - mHead;
    if (norm(dir) <= 2 * mFrom->getRadius()) return;

    sf::Vector2f offset = (mHead != mTail ? dir * (mFrom->getRadius() / norm(dir)) : sf::Vector2f(0, 0));

    mLine[0].position = mHead + offset;
    mLine[1].position = mTail - offset;

    float theta = angle(mHead, mTail);

    mArrowHead.setPoint(0, mTail - offset);
    mArrowHead.setPoint(1, {
        mTail.x - offset.x - mArrowSize * float(std::cos(theta - 3.1415f / 6)),
        mTail.y - offset.y - mArrowSize * float(std::sin(theta - 3.1415f / 6))
    });
    mArrowHead.setPoint(2, {
        mTail.x - offset.x - mArrowSize * float(std::cos(theta + 3.1415f / 6)), 
        mTail.y - offset.y - mArrowSize * float(std::sin(theta + 3.1415f / 6))
    });
}

void Edge::setHead(sf::Vector2f head)
{
    mHead = head;
    buildEdge();
}

void Edge::setTail(sf::Vector2f tail)
{
    mTail = tail;
    buildEdge();
}

void Edge::setColor(sf::Color color)
{
    mColor = color;
    buildEdge();
}

sf::Color Edge::getColor()
{
    return mColor;
}

sf::Vector2f Edge::getHead()
{
    return mHead;
}

sf::Vector2f Edge::getTail()
{
    return mTail;
}

void Edge::update(sf::Time dt)
{
    if (!mIsChangingTail)
    {
        // if (mHead == mFrom->getPosition() && mTail == mTo ? mTo->getPosition() : mFrom->getPosition())
        //     return;

        mHead = mFrom->getPosition();
        mTail = mTo ? mTo->getPosition() : mFrom->getPosition();
        buildEdge();
    }
}

void Edge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mLine, states);
    if (mHasArrow)
        target.draw(mArrowHead, states);
}