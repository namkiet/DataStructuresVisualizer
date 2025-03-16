#include <SceneNode/TreeNode.hpp>

TreeNode::TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    CircleNode(value, radius, fillColor, outlineColor),
    mLeft(nullptr),
    mRight(nullptr),
    mParent(nullptr),
    mHeight(1)
{  
}