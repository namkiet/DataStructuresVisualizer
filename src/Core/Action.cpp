#include <Core/Action.hpp>
#include <iostream>

namespace Action
{
    ActionFunc HighlightNode(CircleNode* node, sf::Color highlightColor, float duration)
    {
        return [node, highlightColor, duration, 
                elapsed = 0.0f, isInit = false,
                startFillColor = sf::Color(), startOutlineColor = sf::Color()](sf::Time dt) mutable -> bool
        {   
            if (!node) return true;
        
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
                std::cerr << "Node " << node->mValue << " is highlighted \n";
                node->setFillColor(startFillColor);
                node->setOutlineColor(startOutlineColor);
                return true;
            }

            return false;
        };
    }

    ActionFunc MoveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect)
    {
        return [node, targetPos, duration, appearEffect, 
                elapsed = 0.0f, isInit = false, opacity = 1.0f,
                startPos = sf::Vector2f(), speed = sf::Vector2f()]
                (sf::Time dt) mutable -> bool
        {
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

            if (speed == sf::Vector2f(0, 0)  || elapsed >= duration) {
                std::cerr << "Node " << node->mValue << " is moved to (" << targetPos.x << " " << targetPos.y << ")\n";

                node->setPosition(targetPos);
                node->setOpacity(1);
                return true;
            }
            return false;
        };
    }

    ActionFunc MoveEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
    {
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

            if (speed == sf::Vector2f(0, 0) || elapsed >= duration)
            {
                std::cerr << "Edge " << parent->mValue << "-" << (child ? child->mValue : -1) << " is switched to " << parent->mValue << "-" << (targetTail ? targetTail->mValue : -1) << "\n";
        
                edge->mTo = targetTail;
                edge->mIsChangingTail = false;
                return true;
            }

            return false;
        };     
    }

    ActionFunc TraverseEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
    {
        return [&edgeList, parent, child, highlightColor, duration, 
            elapsed = 0.0f, isInit = false, 
            edge = static_cast<Edge*>(nullptr), 
            startColor = sf::Color()](sf::Time dt) mutable -> bool 
        {    
            if (!child) return true;

            if (!isInit)
            {   
                edge = Helper::findEdge(edgeList, parent, child);
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
                std::cerr << "Edge " << parent->mValue << "-" << (child ? child->mValue : -1) << " is traversed \n";
            
                edge->setColor(startColor);
                return true;
            }
    
            return false;
        };
    }

    namespace Helper
    {
        Edge* findEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child)
        {
            for (auto& edge : edgeList) {
                if (edge->mFrom == parent && edge->mTo == child) {
                    return edge.get();
                }
            }
            return nullptr;
        }
    }
}