#include "SceneNode/LinkedListNode.hpp"

LinkedListNode::LinkedListNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    CircleNode(value, radius, fillColor, outlineColor),
    mNext(nullptr),
    mPrev(nullptr)
{
}

void LinkedListNode::setNext(LinkedListNode* next)
{
    mNext = next;
}
void LinkedListNode::setPrev(LinkedListNode* prev)
{
    mPrev = prev;
}

LinkedListNode* LinkedListNode::getPrev() const
{
    return mPrev;
}

LinkedListNode* LinkedListNode::getNext() const
{
    return mNext;
}
