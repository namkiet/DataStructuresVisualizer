#include <Core/Action.hpp>

namespace Action
{
    ActionFunc MoveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect) {
        return [node, targetPos, duration, appearEffect, 
                elapsed = 0.0f, isInit = false, opacity = 1.0f,
                startPos = sf::Vector2f(), speed = sf::Vector2f()]
                (sf::Time dt) mutable -> bool {
            if (!node) return true;

            if (!isInit) {
                opacity = appearEffect ? 0 : 1;
                startPos = node->getPosition();
                speed = (targetPos - startPos) / duration;
                isInit = true;
            }

            elapsed += dt.asSeconds();
            sf::Vector2f newPos = startPos + speed * elapsed;
            node->setPosition(newPos);

            if (appearEffect) {
                float t = std::sin((elapsed / duration) * 3.14159f / 2);
                opacity = t;
                node->setOpacity(opacity);
            }

            if (speed == sf::Vector2f(0, 0) || elapsed >= duration) {
                node->setPosition(targetPos);
                node->setOpacity(1);
                return true;
            }
            return false;
        };
    }

    ActionFunc MoveEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration) {
        return [&edgeList, parent, child, targetTail, duration, 
            elapsed = 0.0f, isInit = false, 
            edge = static_cast<Edge*>(nullptr),
            startPos = sf::Vector2f(), targetPos = sf::Vector2f(), 
            speed = sf::Vector2f()](sf::Time dt) mutable -> bool
        {

            if (!isInit)
            {
                edge = Helper::findEdge(edgeList, parent, child);
                if (!edge) return true;

                startPos = edge->getTail();
                targetPos = targetTail ? targetTail->getPosition() : edge->getHead();
                speed = (targetPos - startPos) / duration;
                edge->mIsChangingTail = true;
                isInit = true;
            }


            elapsed += dt.asSeconds();
            sf::Vector2f newPos = startPos + speed * elapsed;
            edge->setTail(newPos);

            if (speed == sf::Vector2f(0, 0) || elapsed >= duration) {
                edge->mTo = targetTail;
                edge->mIsChangingTail = false;
                return true;
            }

            return false;
            };     
    }

    namespace Helper
    {
        Edge* findEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child) {
            for (auto& edge : edgeList) {
                if (edge->mFrom == parent && edge->mTo == child) {
                    return edge.get();
                }
            }
            return nullptr;
        }
    }
}