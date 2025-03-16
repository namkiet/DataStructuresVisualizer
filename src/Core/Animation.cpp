#include <Core/Animation.hpp>
#include <Core/Utility.hpp>
#include <iostream>
#include <math.h>

/*
    START NODE HIGHLIGHT
*/
NodeHighlight::NodeHighlight(TreeNode* node, sf::Color highlightColor, float duration): 
    node(node), highlightColor(highlightColor)
{
    elapsed = 0;
    finished = false;
    isInit = false;

    this->duration = duration;
}

bool NodeHighlight::update(sf::Time dt)
{
    if (finished) return true;

    if (!isInit)
    {   
        startColor = node->getColor();
        isInit = true;
    }

    elapsed += dt.asSeconds();
    float t = std::sin((elapsed / duration) * 3.14159f); // Biến thiên theo sóng sin

    sf::Color newColor(
        int(startColor.r + t * (highlightColor.r - startColor.r)),
        int(startColor.g + t * (highlightColor.g - startColor.g)),
        int(startColor.b + t * (highlightColor.b - startColor.b))
    );

    node->setColor(newColor);

    if (elapsed >= duration)
    {
        node->setColor(startColor);
        finished = true;
    }
 
    return finished;
}

/*
    END NODE HIGHLIGHT
*/


/*------------------------------------------------------------------------------------------------------------*/


/*
    START NODE MOVE
*/

NodeMove::NodeMove(TreeNode* node, sf::Vector2f targetPos, float duration, bool appearEffect): 
    node(node), targetPos(targetPos), hasAppearEffect(appearEffect)
{
    elapsed = 0;
    finished = false;
    isInit = false;

    this->duration = duration;
}

bool NodeMove::update(sf::Time dt)
{
    if (!node) return true;
    if (finished) return true;

    if (!isInit)
    {   
        opacity = hasAppearEffect ? 0 : 1;
        startPos = node->getPosition();
        speed = (targetPos - startPos) / duration;
        isInit = true;
    }

    elapsed += dt.asSeconds();

    sf::Vector2f newPos = startPos + speed * elapsed;
    node->setPosition(newPos);
    
    if (hasAppearEffect)
    {
        float t = std::sin((elapsed / duration) * 3.14159f / 2);
        opacity = t;
        node->setOpacity(opacity);
    }

    if (speed == sf::Vector2f(0, 0) || elapsed >= duration)
    {
        node->setPosition(targetPos);
        node->setOpacity(1);
        finished = true;
    }

    return finished;
}

/*
    END NOVE MOVE
*/


/*------------------------------------------------------------------------------------------------------------*/


/*
    START EDGE MOVE
*/

EdgeMove::EdgeMove(const std::vector<Edge::Ptr> &edges, TreeNode* parent, TreeNode* child, TreeNode* target, float duration):
    edges(edges),
    parent(parent),
    child(child),
    target(target),
    edge(nullptr)
{

    elapsed = 0;
    finished = false;
    isInit = false;

    this->duration = duration;
}

bool EdgeMove::update(sf::Time dt)
{
    if (finished) return true;

    if (!isInit)
    {   
        for (auto& e : edges) {
            if (e->mFrom == parent && e->mTo == child) {
                edge = e.get();
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

    if (speed == sf::Vector2f(0, 0) || elapsed >= duration)
    {
        edge->mTo = target;
        edge->mIsChangingTail = false;
        finished = true;
    }

    return finished;
}


/*
    END EDGE MOVE
*/

/*
    START NODE ASSIGN
*/

NodeAssign::NodeAssign(TreeNode* &a, TreeNode* b): a(a), b(b) {}

bool NodeAssign::update(sf::Time dt)
{
    a = b;
    return true;
}

/* 
    END NODE ASSIGN
*/