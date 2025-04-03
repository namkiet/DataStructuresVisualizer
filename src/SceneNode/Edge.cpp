#include <SceneNode/Edge.hpp>
#include <Core/Utility.hpp>
#include <Core/Variables.hpp>
#include <iostream>

Edge::Edge(sf::Color color, CircleNode* from, CircleNode* to, bool hasArrow, float thickness): 
    mFrom(from),
    mTo(to),
    mLine(sf::Quads, 4),
    mThickness(VIZ::EDGE::Thickness),
    mColor(color),
    mHasArrow(hasArrow),
    mIsChangingTail(false)
{   
    mArrowSize = 12;
    mArrowHead.setPointCount(3);

    mHead = mFrom->getPosition();
    mTail = (mTo ? mTo->getPosition() : mFrom->getPosition());

    updateEdge();
}


void Edge::updateEdge()
{
    mLine[0].color = mColor;
    mLine[1].color = mColor;
    mLine[2].color = mColor;
    mLine[3].color = mColor;
    mArrowHead.setFillColor(mColor);

    sf::Vector2f dir = mTail - mHead;

    sf::Vector2f perp(-dir.y, dir.x);
    perp = perp * ((mThickness / 2) / norm(perp));

    sf::Vector2f offset = dir * (mFrom->getRadius() / norm(dir));
    if (norm(dir) <= 2 * mFrom->getRadius()) 
    {
        offset = sf::Vector2f(0, 0);
        perp = sf::Vector2f(0, 0);
    }

    mLine[0].position = mHead + offset - perp;
    mLine[1].position = mHead + offset + perp;
    mLine[2].position = mTail - offset + perp;
    mLine[3].position = mTail - offset - perp;

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
    updateEdge();
}

void Edge::setTail(sf::Vector2f tail)
{
    mTail = tail;
    updateEdge();
}

void Edge::setColor(sf::Color color)
{
    mColor = color;
    updateEdge();
}

void Edge::setThickness(float thickness)
{
    mThickness = thickness;
    updateEdge();
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
        mHead = mFrom->getPosition();
        mTail = mTo ? mTo->getPosition() : mFrom->getPosition();
        updateEdge();
    }
}

void Edge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mLine, states);
    if (mHasArrow)
        target.draw(mArrowHead, states);
}