#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <memory>

class CircleNode:
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
    typedef std::unique_ptr<CircleNode> Ptr;
                            CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    int                     mValue;

protected:
    sf::CircleShape         mShape;
    sf::Text                mText;
    sf::Font                mFont;
    float                   mTextSize;

public:
    sf::Color               getFillColor();
    void                    setFillColor(sf::Color color);
    sf::Color               getOutlineColor();
    void                    setOutlineColor(sf::Color color);
    void                    setOpacity(float opacity);
    float                   getRadius();
    void                    setValue(int value);
    float                   getTextSize();
    void                    setTextSize(float textSize);   

protected:
    void                    updateText();
};