#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <DataStructures/CircleNode.hpp>

class TreeNode : public CircleNode
{
public:
    typedef std::unique_ptr<TreeNode> Ptr;

public:
                            TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);
    TreeNode*               mLeft;
    TreeNode*               mRight;
    TreeNode*               mParent;
    int                     mHeight;
    int                     mLevel;
};