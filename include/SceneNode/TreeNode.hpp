#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <Core/AnimationQueue.hpp>
#include <Core/AnimationQueue.hpp>
// #include <SceneNode/Edge.hpp>

class TreeNode :
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
                            TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);
                            ~TreeNode();
    int                     mValue;
    TreeNode*               mLeft;
    TreeNode*               mRight;
    TreeNode*               mParent;
    int                     mHeight;
    int                     mLevel;


private:
    virtual void            updateCurrent(sf::Time dt);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::CircleShape         mShape;
    sf::Color               mColor;
    sf::Text                mText;
    sf::Font                font;

public:
    sf::Color               getColor();
    void                    setColor(sf::Color color);
    void                    setOpacity(float opacity);
    float                   getRadius();
};