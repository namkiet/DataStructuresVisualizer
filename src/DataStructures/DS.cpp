#include <DataStructures/DS.hpp>

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto &node: mNodeList)
        if (node) node->draw(target, states);
    
    for (auto &edge: mEdgeList)
        if (edge) edge->draw(target, sf::Transform::Identity);
}

void DS::updateCurrent(sf::Time dt)   
{
    mAnimationQueue.update(dt);

    mActionQueue.update(dt);

    for (auto &node: mNodeList)
        if (node) node->update(dt);
    
    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);
}

void DS::addNode(CircleNode* node)
{
    mActionQueue.pushAction([this, node](sf::Time dt) mutable -> bool
    {
        mNodeList.push_back(CircleNode::Ptr(node));
        return true;
    });
}

void DS::removeNode(CircleNode* node)
{
    node->setPosition(0, 0);
    for (auto& edge : mEdgeList) {
        if (edge->mTo == node) {
            edge->mTo = nullptr;    
        }
    }
}
void DS::addEdge(CircleNode* parent, CircleNode* child, bool hasArrow) 
{
    mActionQueue.pushAction([this, parent, child, hasArrow](sf::Time dt) mutable -> bool
    {
        mEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, parent, child,hasArrow));
        return true;
    });
}

Edge* DS::findEdge(CircleNode* parent, CircleNode* child) {
    for (auto& edge : mEdgeList) {
        if (edge->mFrom == parent && edge->mTo == child) {
            return edge.get();
        }
    }
    return nullptr;
}

void DS::removeEdge(CircleNode* parent, CircleNode* child) {
    mEdgeList.erase(
        std::remove_if(mEdgeList.begin(), mEdgeList.end(),
        [parent, child](const std::unique_ptr<Edge>& edge) {
            return edge->mFrom == parent && edge->mTo == child;
        }),
        mEdgeList.end()
    );
}

void DS::createNewActionGroup()
{
    mActionQueue.createNewBatch();
}

void DS::highlightNode(CircleNode* node, sf::Color highlightColor, float duration)
{

    mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration));
    mActionQueue.pushUndo(Action::HighlightNode(node, highlightColor, duration));
}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect)
{
    if (!node) return;
    sf::Vector2f prevPos = node->getPosition();
    mActionQueue.pushAction(Action::MoveNode(node, targetPos, duration, appearEffect));
    mActionQueue.pushUndo(Action::MoveNode(node, prevPos, duration, false));
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
{
    mActionQueue.pushAction(Action::MoveEdge(mEdgeList, parent, child, targetTail, duration)); 
    mActionQueue.pushUndo(Action::MoveEdge(mEdgeList, parent, targetTail, child, duration));
}

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
    mActionQueue.pushUndo(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
}

#include <iostream>

void DS::undo()
{
    std::cerr << "HELLO\n";
    // mActionQueue.undo();

    removeNode(mNodeList[1].get());
}