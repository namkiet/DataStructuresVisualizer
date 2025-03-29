#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <Core/Animation.hpp>
#include <SceneNode/LinkedListNode.hpp>
class LinkedList: public DS
{
public:
                                LinkedList();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        InsertAtHead(int value);   
    void                        InsertAtLast(int value);
public:
    // virtual void                updateCurrent(sf::Time dt) override;
    // virtual void                drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    LinkedListNode*             mHead;
    // LinkedList                       CreateNode(int value);

    void                        align(LinkedListNode* curNode, sf::Vector2f curPos, float curSpacingX, float curSpacingY);        
};

