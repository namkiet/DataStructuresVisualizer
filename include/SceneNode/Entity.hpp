#pragma once
#include <SceneNode/SceneNode.hpp>

class Entity : public SceneNode
{
private:
    sf::Vector2f        mPosition;

public:
    sf::Vector2f        getPosition() const;

private:
    virtual void        update(sf::Time dt);
};