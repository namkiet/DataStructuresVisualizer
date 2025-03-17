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

void DS::createNewActionGroup()
{
    mActionQueue.createNewBatch();
}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect)
{
    mActionQueue.pushAction([node, targetPos, duration, appearEffect, 
                            elapsed = 0.0f, isInit = false, 
                            startPos = sf::Vector2f(), speed = sf::Vector2f(), opacity = 1.0f](sf::Time dt) mutable -> bool {
        if (!node) return true;

        if (!isInit)
        {
            opacity = appearEffect ? 0 : 1;
            startPos = node->getPosition();
            speed = (targetPos - startPos) / duration;
            isInit = true;
        }

        elapsed += dt.asSeconds();

        sf::Vector2f newPos = startPos + speed * elapsed;
        node->setPosition(newPos);

        if (appearEffect)
        {
            float t = std::sin((elapsed / duration) * 3.14159f / 2);
            opacity = t;
            node->setOpacity(opacity);
        }

        if (speed == sf::Vector2f(0, 0) || elapsed >= duration)
        {
            node->setPosition(targetPos);
            node->setOpacity(1);
            return true;
        }

        return false;
    });
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* target, float duration)
{
    mActionQueue.pushAction([this, parent, child, target, duration, 
                                elapsed = 0.0f, isInit = false, 
                                edge = static_cast<Edge*>(nullptr), 
                                startPos = sf::Vector2f(), targetPos = sf::Vector2f(), 
                                speed = sf::Vector2f()](sf::Time dt) mutable -> bool {
        if (!isInit)
        {
            for (auto &e : mEdgeList) {
                if (e->mFrom == parent && e->mTo == child) {
                    edge = e.get();
                    break;
                }
            }

            if (!edge) return true;

            startPos = edge->getTail();
            targetPos = target ? target->getPosition() : edge->getHead();
            speed = (targetPos - startPos) / duration;
            isInit = true;
            edge->mIsChangingTail = true;
        }

        if (!edge) return true;

        elapsed += dt.asSeconds();
        sf::Vector2f newPos = startPos + speed * elapsed;
        edge->setTail(newPos);

        if (speed == sf::Vector2f(0, 0) || elapsed >= duration) {
            edge->mTo = target;
            edge->mIsChangingTail = false;
            return true;
        }

        return false;
    });        
}