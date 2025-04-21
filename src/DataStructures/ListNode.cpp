#include <DataStructures/ListNode.hpp>

ListNode::ListNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    CircleNode(value, radius, fillColor, outlineColor),
    mNext(nullptr),
    mPrev(nullptr)
{
}

void ListNode::setNext(ListNode* next)
{
    mNext = next;
}
void ListNode::setPrev(ListNode* prev)
{
    mPrev = prev;
}

ListNode* ListNode::getPrev() const
{
    return mPrev;
}

ListNode* ListNode::getNext() const
{
    return mNext;
}
