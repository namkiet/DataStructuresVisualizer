#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <DataStructures/DS.hpp>

class HeapTree: public DS
{
public:
                            HeapTree();
    void                    insert(int value) override;
    void                    remove(int value) override;
    bool                    search(int value) override;
    void                    empty() override;

private:
    TreeNode*               mRoot;

private:
    void                    align(int index, sf::Vector2f curPos = sf::Vector2f(600, 100), float curSpacingX = 300, float curSpacingY = 100); 
    void                    heapifyUp(int index); 
    void                    heapifyDown(int index); 
};