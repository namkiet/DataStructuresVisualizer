#include <SceneNode/Edge.hpp>
#include <Core/Utility.hpp>

Edge::Edge(sf::Color color, sf::Vector2f head, sf::Vector2f tail): 
    mLine(sf::PrimitiveType::LineStrip, 2),
    mColor(color),
    mHead(head),
    mTail(tail),
    mHasArrow(false)
{   
    mArrowSize = 12;
    mArrowHead.setPointCount(3);
    buildEdge();
}

void Edge::buildEdge()
{
    mLine[0].color = mColor;
    mLine[1].color = mColor;
    mLine[0].position = mHead; 
    mLine[1].position = mTail;

    float theta = angle(mHead, mTail);

    mArrowHead.setFillColor(mColor);
    mArrowHead.setPoint(0, mTail);
    mArrowHead.setPoint(1, {
        mTail.x - mArrowSize * float(std::cos(theta - 3.1415f / 6)),
        mTail.y - mArrowSize * float(std::sin(theta - 3.1415f / 6))
    });
    mArrowHead.setPoint(2, {
        mTail.x - mArrowSize * float(std::cos(theta + 3.1415f / 6)), 
        mTail.y - mArrowSize * float(std::sin(theta + 3.1415f / 6))
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

void Edge::draw(sf::RenderTarget &target, sf::RenderStates states) const
{   
    target.draw(mLine, states);
    if (mHasArrow)
        target.draw(mArrowHead, states);
}