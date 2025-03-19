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
    mNodeList.erase(
        std::remove_if(mNodeList.begin(), mNodeList.end(),
        [node](const CircleNode::Ptr& cur) {
            return cur.get() == node;
        }),
        mNodeList.end()
    );
}

void DS::addEdge(CircleNode* parent, CircleNode* child) 
{
    mActionQueue.pushAction([this, parent, child](sf::Time dt) mutable -> bool
    {
        mEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, parent, child, false));
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
    mActionQueue.pushAction([node, highlightColor, duration, 
                            elapsed = 0.0f, isInit = false,
                            startFillColor = sf::Color(), startOutlineColor = sf::Color()](sf::Time dt) mutable -> bool
    {   
        if (!isInit)
        {   
            startFillColor = node->getFillColor();
            startOutlineColor = node->getOutlineColor();
            isInit = true;
        }

        elapsed += dt.asSeconds();
        float t = std::sin((elapsed / duration) * 3.14159f); // Biến thiên theo sóng sin

        sf::Color newFillColor(
            int(startFillColor.r + t * (highlightColor.r - startFillColor.r)),
            int(startFillColor.g + t * (highlightColor.g - startFillColor.g)),
            int(startFillColor.b + t * (highlightColor.b - startFillColor.b))
        );

        sf::Color newOutlineColor(
            int(startOutlineColor.r + t * (highlightColor.r - startOutlineColor.r)),
            int(startOutlineColor.g + t * (highlightColor.g - startOutlineColor.g)),
            int(startOutlineColor.b + t * (highlightColor.b - startOutlineColor.b))
        );

        node->setFillColor(newFillColor);
        node->setOutlineColor(newOutlineColor);

        if (elapsed >= duration)
        {
            node->setFillColor(startFillColor);
            node->setOutlineColor(startOutlineColor);
            return true;
        }

        return false;
    });
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
    mActionQueue.pushAction([this, parent, child, highlightColor, duration, 
        elapsed = 0.0f, isInit = false, 
        edge = static_cast<Edge*>(nullptr), 
        startColor = sf::Color()](sf::Time dt) mutable -> bool 
    {
        if (!child) return true;

        if (!isInit)
        {   
            edge = findEdge(parent, child);
            if (!edge) return true;

            startColor = edge->getColor();
            isInit = true;
        }

        elapsed += dt.asSeconds();
        float t = std::sin((elapsed / duration) * 3.14159f); // Biến thiên theo sóng sin

        sf::Color newColor(
            int(startColor.r + t * (highlightColor.r - startColor.r)),
            int(startColor.g + t * (highlightColor.g - startColor.g)),
            int(startColor.b + t * (highlightColor.b - startColor.b))
        );


        edge->setColor(newColor);

        if (elapsed >= duration)
        {
            edge->setColor(startColor);
            return true;
        }

        return false;
    });
}

#include <iostream>

void DS::undo()
{
    std::cerr << "HELLO\n";
    mActionQueue.undo();
}