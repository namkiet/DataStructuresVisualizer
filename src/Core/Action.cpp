#include <Core/Action.hpp>
#include <Core/Variables.hpp>
#include <iostream>
#include <cmath>

namespace Action
{
    ActionFunc Wait(float duration)
    {
        return [duration, elapsed = 0.f](sf::Time dt) mutable -> bool
        {
            elapsed += dt.asSeconds() * ANIMATION::Speed;
            if (elapsed >= duration || ANIMATION::Speed >= 1000)
                return true;
            return false;
        };
    }

    ActionFunc HighlightNode(CircleNode* node, sf::Color highlightFillColor, float duration, bool reverse)
    {
        return [node, highlightFillColor, duration, reverse,
                elapsed = 0.0f, isInit = false,
                startFillColor = sf::Color(), startOutlineColor = sf::Color(), highlightOutlineColor = sf::Color()](sf::Time dt) mutable -> bool
        {   
            if (!node) return true;
        
            if (!isInit)
            {   
                startFillColor = node->getFillColor();
                startOutlineColor = node->getOutlineColor();

                if (highlightFillColor == VIZ::NODE::FillColor)
                    highlightOutlineColor = VIZ::NODE::OutlineColor;
                else
                    highlightOutlineColor = highlightFillColor;

                if (highlightFillColor == startFillColor && highlightOutlineColor == startOutlineColor)
                    return true;

                isInit = true;
            }

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f * (reverse ? 1.f : 0.5f)); // Biến thiên theo sóng sin

            sf::Color newFillColor(
                int(startFillColor.r + t * (highlightFillColor.r - startFillColor.r)),
                int(startFillColor.g + t * (highlightFillColor.g - startFillColor.g)),
                int(startFillColor.b + t * (highlightFillColor.b - startFillColor.b)),
                int(startFillColor.a + t * (highlightFillColor.a - startFillColor.a))
            );

            sf::Color newOutlineColor(
                int(startOutlineColor.r + t * (highlightOutlineColor.r - startOutlineColor.r)),
                int(startOutlineColor.g + t * (highlightOutlineColor.g - startOutlineColor.g)),
                int(startOutlineColor.b + t * (highlightOutlineColor.b - startOutlineColor.b)),
                int(startOutlineColor.a + t * (highlightOutlineColor.a - startOutlineColor.a))
            );

            node->setFillColor(newFillColor);
            node->setOutlineColor(newOutlineColor);

            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                node->setFillColor(reverse ? startFillColor : highlightFillColor);
                node->setOutlineColor(reverse ? startOutlineColor : highlightOutlineColor);
                return true;
            }

            return false;
        };
    }

    ActionFunc ChangeNodeColor(CircleNode* node, sf::Color highlightColor, float duration) // highlight node and change to the highlight color
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

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f / 2); // Biến thiên theo sóng sin

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

            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                // std::cerr << "Node " << node->mValue << " is highlighted \n";
                return true;
            }

            return false;
        };
    }

    ActionFunc MoveNode(CircleNode* node, sf::Vector2f targetPos, float duration)
    {
        return [node, targetPos, duration,
            elapsed = 0.0f, isInit = false, startPos = sf::Vector2f(), speed = sf::Vector2f()] (sf::Time dt) mutable -> bool
        {
            if (!node) return true;
            
            if (!isInit) {
                startPos = node->getPosition();
                speed = (targetPos - startPos) / duration;
                isInit = true;
            }

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            sf::Vector2f newPos = startPos + speed * elapsed;
            node->setPosition(newPos);

            if (speed == sf::Vector2f(0, 0)  || elapsed >= duration || ANIMATION::Speed >= 1000) 
            {
                node->setPosition(targetPos);
                return true;
            }
            return false;
        };
    }

    ActionFunc FadeInNode(CircleNode* node, float duration)
    {
        return [node, duration, 
            elapsed = 0.f, opacity = 0.f, isInit = false](sf::Time dt) mutable->bool 
        {
            if (!isInit)
            {
                if (node->getOpacity() == 1.f) return true;
                isInit = true;
            }

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f / 2);
            opacity = t;
            node->setOpacity(opacity);

            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                node->setOpacity(1);
                return true;
            }
            return false;
        };
    }

    ActionFunc FadeOutNode(CircleNode* node, float duration)
    {
        return [node, duration, 
            elapsed = 0.f, opacity = 1.f, isInit = false](sf::Time dt) mutable->bool 
        {
            if (!isInit)
            {
                if (node->getOpacity() == 0.f) return true;
                isInit = true;
            }

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::cos((elapsed / duration) * 3.14159f / 2);
            opacity = t;
            node->setOpacity(opacity);
            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                node->setOpacity(0);
                return true;
            }
            return false;
        };
    }

    ActionFunc ChangeNodeValue(CircleNode* node, float targetValue, float duration)
    {
        return [node, targetValue, duration, 
            elapsed = 0.f, isInit = false, startTextSize = 0.f]
            (sf::Time dt) mutable ->bool
        {
            if (!isInit)
            {
                startTextSize = node->getTextSize();
                // node->mValue = targetValue;
                isInit = true;
            }

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f);

            if (elapsed >= duration / 2)
                node->setValue(targetValue);
                
            node->setTextSize(startTextSize * (1 - t));

            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                node->setValue(targetValue);
                node->setTextSize(startTextSize);
                return true;
            }

            return false;
        };
    }

    ActionFunc SwapNodeValues(CircleNode* nodeA, CircleNode* nodeB, float duration)
    {
        int valueA = nodeA->mValue;
        int valueB = nodeB->mValue;

        ActionFunc changeA = ChangeNodeValue(nodeA, valueB, duration);
        ActionFunc changeB = ChangeNodeValue(nodeB, valueA, duration);

        return [changeA, changeB](sf::Time dt) mutable -> bool
        {
            bool doneA = changeA(dt);
            bool doneB = changeB(dt);
            return doneA && doneB;
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

            elapsed += dt.asSeconds() * ANIMATION::Speed;
            sf::Vector2f newPos = startPos + speed * elapsed;
            edge->setTail(newPos);

            if (speed == sf::Vector2f(0, 0) || elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                edge->setTail(targetPos);
                edge->mTo = targetTail;
                edge->mIsChangingTail = false;
                return true;
            }

            return false;
        };     
    }

    ActionFunc TraverseEdge(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration, bool reverse)
    {
        return [&edgeList, parent, child, highlightColor, duration, reverse,
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
    
            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f * (reverse ? 1.f : 0.5f)); // Biến thiên theo sóng sin
    
            sf::Color newColor(
                int(startColor.r + t * (highlightColor.r - startColor.r)),
                int(startColor.g + t * (highlightColor.g - startColor.g)),
                int(startColor.b + t * (highlightColor.b - startColor.b)),
                int(startColor.a + t * (highlightColor.r - startColor.a))
            );
    
    
            edge->setColor(newColor);
    
            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                edge->setColor(reverse ? startColor : highlightColor);
                return true;
            }
    
            return false;
        };
    }

    ActionFunc DeleteNode(CircleNode* node, float duration)
    {
        return[node, duration, 
            elapsed = 0.0f, isInit = false](sf::Time dt) mutable -> bool
        {
            if (!node) return true;

            if (!isInit)
            {
                node->setOpacity(1);
                isInit = true;
            }

            elapsed += dt.asSeconds();
            float t = std::sin((elapsed / duration) * 3.14159f / 2);

            float curOpacity = std::max(0.0f, 1 - t);
            node->setOpacity(curOpacity);

            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                node->setOpacity(0);
                return true;
            }

            return false;
        };
    }

    ActionFunc ChangeEdgeColor(std::vector<Edge::Ptr> &edgeList, CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
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
    
            elapsed += dt.asSeconds() * ANIMATION::Speed;
            float t = std::sin((elapsed / duration) * 3.14159f / 2); // Biến thiên theo sóng sin
    
            sf::Color newColor(
                int(startColor.r + t * (highlightColor.r - startColor.r)),
                int(startColor.g + t * (highlightColor.g - startColor.g)),
                int(startColor.b + t * (highlightColor.b - startColor.b))
            );
    
    
            edge->setColor(newColor);
    
            if (elapsed >= duration || ANIMATION::Speed >= 1000)
            {
                // std::cerr << "Edge " << parent->mValue << "-" << (child ? child->mValue : -1) << " is traversed \n";
                edge->setColor(highlightColor);
                return true;
            }
    
            return false;
        };
    }

    Action::ActionFunc MarkEdge(Edge* edge, int direction, float duration)
{
    return [edge, duration, 
            elapsed = 0.f, isInit = false, direction
            ](sf::Time dt) mutable -> bool
    {
        if (!edge) return true;

        if(direction == -1 && isInit == false){
            // swap mHead and mTail
            edge->swapEndpoint();
            isInit = true;
            edge->mIsChangingTail = true;
        }
        sf::Vector2f head = edge->getHead();
        sf::Vector2f tail = edge->getTail();
        
        elapsed += dt.asSeconds() * ANIMATION::Speed;
        float t = elapsed / duration;
        t = std::min(t, 1.0f);

        sf::Vector2f mid = head + (tail - head) * t;
        edge->setMid(mid);

        if (elapsed >= duration || ANIMATION::Speed >= 1000)
        {
            if(direction == -1){
                // swap mHead and mTail again
                edge->swapEndpoint();
                edge->mIsChangingTail = false;
            }
            edge->resetMid();
            edge->setColor(edge->getMarkColor());

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