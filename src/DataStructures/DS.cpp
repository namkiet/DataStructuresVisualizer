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

    for (auto &node: mNodeList)
        if (node) node->update(dt);
    
    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);
}

void DS::addNode(CircleNode* node)
{
    mNodeList.push_back(CircleNode::Ptr(node));
}

void DS::removeNode(CircleNode* node)
{
    mNodeList.erase(
        std::remove_if(mNodeList.begin(), mNodeList.end(),
        [node](const CircleNode::Ptr& cur) {
            return cur.get() == node;
        }),
        mNodeList.end()
    );
}

void DS::addEdge(CircleNode* parent, CircleNode* child) {
    mEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, parent, child, false));
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