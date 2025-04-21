#pragma once
#include <SceneNode/SceneNode.hpp>
#include <DataStructures/TreeNode.hpp>
#include <DataStructures/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>

class AVLTree: public DS
{
public:
                                AVLTree();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        updateValue(int value, int newValue) override;
    void                        empty() override;

private:
    TreeNode*                   mRoot;
    void                        insertHelper(TreeNode* &node, TreeNode* prev, int value);
    void                        removeHelper(TreeNode* &node, int value);
    bool                        searchHelper(TreeNode* node, int value);
    int                         getHeight(TreeNode* root);
    int                         getBalanceFactor(TreeNode* root);
    TreeNode*                   updateHeight(TreeNode* root);
    void                        balance(TreeNode* &root);
    TreeNode*                   leftRotate(TreeNode* root);
    TreeNode*                   rightRotate(TreeNode* root);

private:
    bool                        isUpdating = false;

private:
    void                        align() override { align(mRoot); };
    void                        align(TreeNode* curNode, sf::Vector2f curPos = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing), float curSpacingX = VIZ::DS::Size.x / 4, float curSpacingY = VIZ::DS::RowSpacing);        
};