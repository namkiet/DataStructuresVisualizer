#include <SceneNode/Edge.hpp>
#include <Core/Utility.hpp>
#include<iostream>
#include <Core/Utility.hpp>

#include <Core/Variables.hpp>
Edge::Edge(sf::Color color, CircleNode* from, CircleNode* to, bool hasArrow, float thickness): 
    mFrom(from),
    mTo(to),
    mLine1(sf::Quads, 4),
    mLine2(sf::Quads,4),
    mThickness(thickness),
    mColor(color),
    mHasArrow(hasArrow),
    mIsChangingTail(false),
    isReversed(false)
{   
    mArrowSize = 12;
    mArrowHead.setPointCount(3);

    mHead = mFrom->getPosition();
    mTail = (mTo ? mTo->getPosition() : mFrom->getPosition());

    updateEdge();
}


Edge::Edge(sf::Color color, CircleNode* from, CircleNode* to, int weight, bool hasArrow, float thickness): 
    mFrom(from),
    mTo(to),
    mLine1(sf::Quads, 4),
    mLine2(sf::Quads, 4),
    mThickness(thickness),
    mColor(color),
    mHasArrow(hasArrow),
    mIsChangingTail(false),
    mWeight(weight),
    mHasWeight(true),
    isReversed(false)
{   
    mFont.loadFromFile("assets/fonts/jetbrains.ttf");
    mWeightText.setFont(mFont);
    sf::Vector2f mid = (mFrom->getPosition() + mTo->getPosition())/2.f;
    sf::Vector2f vec = NormalUnitVector(mTo->getPosition() - mFrom->getPosition());
    vec = sf::Vector2f(vec.x * 5.f, vec.y * 5.f);

    mWeightText.setPosition(mid + vec);
    mWeightText.setCharacterSize(13);
    mWeightText.setColor(sf::Color::Black);
    mWeightText.setString(std::to_string(mWeight));

    mArrowSize = 12;
    mArrowHead.setPointCount(3);

    mHead = mFrom->getPosition();
    mMid = mHead;
    mTail = (mTo ? mTo->getPosition() : mFrom->getPosition());

    updateEdge();
}


int Edge::getWeight(){
    if(mHasWeight == false) std::cerr<<"Edge has no weight"<<std::endl;
    return mWeight;
}


void Edge::updateEdge()
{  
    if(mTail == mHead) return;
    // update mHead mTail mMid to remain the same proportion
    if(isReversed){
        double proportion = dist(mMid, mHead) / dist(mTail, mHead);
        mHead = mTo->getPosition();
        mTail = mFrom->getPosition();
        mMid = mHead + (mTail - mHead) * proportion;
    }
    else{

        double proportion = dist(mMid, mHead) / dist(mTail, mHead);
        mHead = mFrom->getPosition();
        mTail = mTo->getPosition();
        mMid = mHead + (mTail - mHead) * proportion;
    }


    mLine1[0].color = mMarkColor;
    mLine1[1].color = mMarkColor;
    mLine1[2].color = mMarkColor;
    mLine1[3].color = mMarkColor;

    sf::Vector2f dir = mTail - mHead;

    sf::Vector2f perp(-dir.y, dir.x);
    perp = perp * ((mThickness / 2) / norm(perp));

    sf::Vector2f offset = dir * (mFrom->getRadius() / norm(dir));
    if (norm(dir) <= 2 * mFrom->getRadius()) 
    {
        offset = sf::Vector2f(0, 0);
        perp = sf::Vector2f(0, 0);
    }

    mLine1[0].position = mHead + offset - perp;
    mLine1[1].position = mHead + offset + perp;
    mLine1[2].position = mMid - offset + perp;
    mLine1[3].position = mMid - offset - perp;

    float theta = angle(mHead, mTail);

    mLine2[0].color =mColor;
    mLine2[1].color =mColor;
    mLine2[2].color =mColor;   
    mLine2[3].color =mColor;
    
    mLine2[0].position = mLine1[2].position;
    mLine2[1].position = mLine1[3].position;
    mLine2[2].position = mTail - offset + perp;
    mLine2[3].position = mTail - offset - perp;

    // set Arrow
    sf::Color ArrowColor = (mMid != mTail)? mColor : mMarkColor;
    mArrowHead.setFillColor(ArrowColor);
    mArrowHead.setPoint(0, mTail - offset);
    mArrowHead.setPoint(1, {
        mTail.x - offset.x - mArrowSize * float(std::cos(theta - 3.1415f / 6)),
        mTail.y - offset.y - mArrowSize * float(std::sin(theta - 3.1415f / 6))
    });
    mArrowHead.setPoint(2, {
        mTail.x - offset.x - mArrowSize * float(std::cos(theta + 3.1415f / 6)), 
        mTail.y - offset.y - mArrowSize * float(std::sin(theta + 3.1415f / 6))
    });

    sf::Vector2f mid = (mFrom->getPosition() + mTo->getPosition())/2.f;
    sf::Vector2f vec = NormalUnitVector(mTo->getPosition() - mFrom->getPosition());
    vec = sf::Vector2f(vec.x * 5.f, vec.y * 5.f);

    mWeightText.setPosition(mid + vec);


}

void Edge::resetColor()
{
    mMid = (isReversed)? mTail: mHead;
    mColor = VIZ::EDGE::Color;
    updateEdge();

}

void Edge::resetMid()
{
    mMid = (isReversed)? mTail : mHead;
    updateEdge();
}
void Edge::setHead(sf::Vector2f head)
{
    mHead = head;
    updateEdge();
}

void Edge::setMid(sf::Vector2f mid){
    mMid = mid;
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
sf::Color Edge::getMarkColor(){
    return mMarkColor;
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
    target.draw(mLine1, states);
    target.draw(mLine2,states);
    if (mHasArrow)
        target.draw(mArrowHead, states);
    if(mHasWeight){
        target.draw(mWeightText, states);
    }
}

double Edge::lengthEdge(){
    return dist(mHead, mTail);
}
void Edge::swapEndpoint(){
    isReversed = !isReversed;
    sf::Vector2f temp = mHead;
    mHead = mTail;
    mTail = temp;
}