#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>

class HeapTree: public SceneNode
{
public:
                            HeapTree();
    TreeNode*               mRoot;
    std::vector<TreeNode*>  mHeapArray;

public:
    void                    insert(int value);
    // void                    remove(int value);
    bool                    search(int value);

    // void                    leftRotate();
    // void                    rightRotate();

private:
    TreeNode*               insert(TreeNode* node, TreeNode* prev, int value);
    // TreeNode*               remove(TreeNode* node, int value);
    bool                    search(TreeNode* node, int value);

public:
    virtual void            updateCurrent(sf::Time dt);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    AnimationQueue          mAnimationQueue;

private:
    // void                    align(TreeNode* root);
    void                    align();
};