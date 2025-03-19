#pragma once
#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include <functional>

namespace Action
{
    using ActionFunc = std::function<bool(sf::Time)>;

    ActionFunc MoveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect);
    ActionFunc MoveEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);

    namespace Helper
    {
        Edge* findEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child);
    }
}