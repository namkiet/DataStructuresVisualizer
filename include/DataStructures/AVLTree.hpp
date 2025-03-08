#pragma once

#include <SceneNode/TreeNode.hpp>

class AVLTree: 
// public SceneNode
public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
                            AVLTree();
    TreeNode*               mRoot;

public:
    void                    insert(int value);
    // void                    remove(int value);
    bool                    search(int value);

private:
    TreeNode*               insert(TreeNode* node, TreeNode* prev, int value);
    // TreeNode*               remove(TreeNode* node, int value);
    bool                    search(TreeNode* node, int value);
    int                     getHeight(TreeNode* root);
    int                     getBalanceFactor(TreeNode* root);
    TreeNode*               updateHeight(TreeNode* root);
    TreeNode*               leftRotate(TreeNode* root);
    TreeNode*               rightRotate(TreeNode* root);
    TreeNode*               balance(TreeNode* root);

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    AnimationQueue          mAnimationQueue;
};