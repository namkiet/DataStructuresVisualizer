#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>

class AVLTree: public DS
{
public:
                                AVLTree();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        empty() override;

// private:
    // TreeNode*                    rawInsert(int value) override;
    // TreeNode*                    rawInsertHelper(TreeNode* node, TreeNode* prev, int value);

    // Just for debug
    void                        leftRotate();
    void                        rightRotate();

private:
    TreeNode*                   mRoot;
    TreeNode*                   insert(TreeNode* node, TreeNode* prev, int value);
    TreeNode*                   remove(TreeNode* node, int value);
    bool                        search(TreeNode* node, int value);
    int                         getHeight(TreeNode* root);
    int                         getBalanceFactor(TreeNode* root);
    TreeNode*                   updateHeight(TreeNode* root);
    TreeNode*                   leftRotate(TreeNode* root);
    TreeNode*                   rightRotate(TreeNode* root);
    TreeNode*                   balance(TreeNode* root);
    void                        align(TreeNode* curNode, sf::Vector2f curPos = sf::Vector2f(600, 100), float curSpacingX = 300, float curSpacingY = 100);        

public:
    void                        saveState() override;
    void                        loadState(History history) override;
};