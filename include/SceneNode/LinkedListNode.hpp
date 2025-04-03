#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/CircleNode.hpp>

class LinkedListNode : public CircleNode
{
public:
                            LinkedListNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

public:
    void                    setNext(LinkedListNode* next);
    void                    setPrev(LinkedListNode* prev);
    LinkedListNode*         getNext() const;
    LinkedListNode*         getPrev() const;

public:
    LinkedListNode*         mNext;
    LinkedListNode*         mPrev;
    int                     mIndex;
};

