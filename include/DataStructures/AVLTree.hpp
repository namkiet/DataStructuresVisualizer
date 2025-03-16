#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/Edge.hpp>

class AVLTree: public SceneNode
{
public:
                                AVLTree();
    TreeNode*                   mRoot;

public:
    void                        insert(int value);
    void                        remove(int value);
    bool                        search(int value);

    void                        leftRotate();
    void                        rightRotate();

private:
    TreeNode*                   insert(TreeNode* node, TreeNode* prev, int value);
    TreeNode*                   remove(TreeNode* node, int value);
    bool                        search(TreeNode* node, int value);
    int                         getHeight(TreeNode* root);
    int                         getBalanceFactor(TreeNode* root);
    TreeNode*                   updateHeight(TreeNode* root);
    TreeNode*                   leftRotate(TreeNode* root);
    TreeNode*                   rightRotate(TreeNode* root);
    TreeNode*                   balance(TreeNode* root);

public:
    virtual void                updateCurrent(sf::Time dt);
    virtual void                drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    AnimationQueue              mAnimationQueue;

private:
    void                        align(TreeNode* root);

private:
    std::vector<Edge::Ptr>      edges;
    void                        addEdge(TreeNode* parent, TreeNode* child);
    Edge*                       findEdge(TreeNode* parent, TreeNode* child);
    void                        removeEdge(TreeNode* parent, TreeNode* child);         
};