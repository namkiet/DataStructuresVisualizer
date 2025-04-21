#pragma once
#include <SceneNode/SceneNode.hpp>
#include <DataStructures/TreeNode.hpp>
#include <DataStructures/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <DataStructures/ListNode.hpp>

class LinkedList: public DS
{
public:
                                LinkedList();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        empty() override;
    void                        updateValue(int value, int newValue) override;

    void                        InsertAtHead(int value);   
    void                        InsertAtLast(int value);
    void                        insertAtIndex(int value, int index);

private:
    ListNode*             mHead;
    void                        align() override { align(mHead); };
    void                        align(ListNode* curNode, sf::Vector2f curPos = sf::Vector2f(100, 100));

private:
    int                         maxRowNode;
};

