#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <DataStructures/CircleNode.hpp>

class ListNode : public CircleNode
{
public:
                            ListNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

public:
    void                    setNext(ListNode* next);
    void                    setPrev(ListNode* prev);
    ListNode*               getNext() const;
    ListNode*               getPrev() const;

public:
    ListNode*               mNext;
    ListNode*               mPrev;
    int                     mIndex;
};

