#include <SceneNode/TreeNode.hpp>

TreeNode::TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    CircleNode(value, radius, fillColor, outlineColor),
    mLeft(nullptr),
    mRight(nullptr),
    mParent(nullptr),
    mHeight(1)
{  
}

TreeNode::TreeNode(const TreeNode &other) 
    : CircleNode(other), mHeight(other.mHeight), mLevel(other.mLevel), mParent(nullptr) 
{
    mLeft = (other.mLeft) ? new TreeNode(*other.mLeft) : nullptr;
    mRight = (other.mRight) ? new TreeNode(*other.mRight) : nullptr;

    if (mLeft) mLeft->mParent = this;
    if (mRight) mRight->mParent = this;
}