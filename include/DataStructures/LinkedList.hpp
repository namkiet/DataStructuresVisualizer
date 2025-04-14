#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <SceneNode/LinkedListNode.hpp>

class LinkedList: public DS
{
public:
                                LinkedList();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        empty() override;
    void                        InsertAtHead(int value);   
    void                        InsertAtLast(int value);
    void                        insertAtIndex(int value, int index);

private:
    LinkedListNode*             mHead;
    void                        align() override { align(mHead); };
    void                        align(LinkedListNode* curNode, sf::Vector2f curPos = sf::Vector2f(100, 100));

private:
    int                         maxRowNode;
};

