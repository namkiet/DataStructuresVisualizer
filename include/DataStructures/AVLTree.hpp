#pragma once

#include <SceneNode/TreeNode.hpp>

class AVLTree: 
// public SceneNode
public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
                        AVLTree();
    TreeNode*           mRoot;

public:
    void                insert(int value);
    // void                remove(int value);
    bool                search(int value);

private:
    TreeNode*           insert(TreeNode* node, TreeNode* prev, int value);
    // TreeNode*           remove(TreeNode* node, int value);
    bool                search(TreeNode* node, int value);

    void updateLevel(TreeNode* node, int level);
    void updateHeight(TreeNode* node, int level);
    void                    lR(TreeNode* &root);
    void                    rR(TreeNode* &root);

public:
    void                    leftRotate();
    void                    rightRotate();


public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    void                    align(TreeNode* node);


private:
    AnimationQueue          mAnimationQueue;
};