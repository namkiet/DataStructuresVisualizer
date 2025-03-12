#pragma once
#include <Core/AnimationQueue.hpp>
#include <SceneNode/TreeNode.hpp>

// Highlight node
class NodeHighlight : public Animation {
private:
    TreeNode*                   node;
    sf::Color                   startColor;
    sf::Color                   highlightColor;

public:
                                NodeHighlight(TreeNode* node, sf::Color highlightColor, float duration);
    bool                        update(sf::Time dt) override;
};

// Move node
class NodeMove : public Animation {
private:
    TreeNode*                   node;
    sf::Vector2f                startPos;
    sf::Vector2f                targetPos;
    sf::Vector2f                speed;

private:
    float                       opacity;
    bool                        hasAppearEffect;

public:
                                NodeMove(TreeNode* node, sf::Vector2f targetPos, float duration, bool appearEffect = false);
    bool                        update(sf::Time dt) override;
};

// Move edge
class EdgeMove : public Animation {
private:
    Edge*                       edge;
    sf::Vector2f                startPos;
    sf::Vector2f                targetPos;
    sf::Vector2f                speed;

public:
                                EdgeMove(Edge &edge, sf::Vector2f targetPos, float duration);
    bool                        update(sf::Time dt) override;
};