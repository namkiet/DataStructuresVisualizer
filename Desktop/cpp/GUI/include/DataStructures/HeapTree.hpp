#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <DataStructures/DS.hpp>
#include <Core/Animation.hpp>

class HeapTree: public DS
{
public:
                            HeapTree();

public:
    void                    insert(int value) override;
    void                    remove(int value) override;
    bool                    search(int value) override;

private:
    TreeNode*               mRoot;

private:
    void                    align(int index, sf::Vector2f curPos = sf::Vector2f(600, 100), float curSpacingX = 300, float curSpacingY = 100);  
};