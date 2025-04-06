#pragma once
#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include <functional>

namespace Action
{
    using ActionFunc = std::function<bool(sf::Time)>;

    ActionFunc Wait(float duration);
    ActionFunc HighlightNode(CircleNode* node, sf::Color highlightFillColor, float duration, bool reverse);
    ActionFunc MoveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect);
    ActionFunc ChangeNodeValue(CircleNode* node, float targetValue, float duration);
    ActionFunc SwapNodeValues(CircleNode* nodeA, CircleNode* nodeB, float duration);
    ActionFunc FadeNode(CircleNode* node, float duration);
    // ActionFunc SetNoteForNode(CircleNode* node, std::string note, float duration);
    ActionFunc MoveEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);
    ActionFunc TraverseEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration); // Hightlight edge
    ActionFunc DeleteNode(CircleNode* node, float duration);

    namespace Helper
    {
        Edge* findEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child);
    }
}