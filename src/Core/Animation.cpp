#include <Core/Animation.hpp>
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
        startColor.r + t * (highlightColor.r - startColor.r),
        startColor.g + t * (highlightColor.g - startColor.g),
        startColor.b + t * (highlightColor.b - startColor.b)
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

    if (elapsed >= duration)
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