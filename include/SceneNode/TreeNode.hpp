#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>

class TreeNode : 
// public SceneNode 
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
                            TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

private:
    int                     mValue;
    sf::CircleShape         mShape;
    sf::Text                mText;
    sf::Font                font;

public:
    int                     getValue();
    void                    setValue(int value);

public:
    virtual void            update(sf::Time dt) {};
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const {};
    virtual void            updateCurrent(sf::Time dt);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    bool                    mIsAnimationFinished;

private:
    bool                    mIsMoving;
    sf::Vector2f            mTargetPosition;

public:
    void                    moveTo(sf::Vector2f target);

private:
    bool                    mIsHighlighted;
    sf::Color               mTargetColor;
    float                   mElapsedTime;
    float                   mDuration;

public:
    void                    highlight(sf::Color targetColor, float elapsedTime, float duration);
};