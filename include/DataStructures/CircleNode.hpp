#pragma once
#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <memory>

class CircleNode:
    public sf::Transformable, public sf::Drawable
{
public:
    typedef std::shared_ptr<CircleNode> Ptr;
                            CircleNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    int                     mValue;
    sf::Vector2f            mTargetPosition;

protected:
    sf::CircleShape         mShape;
    sf::Text                mText;
    sf::Text                mNote;
    sf::Font                mFont;
    float                   mTextSize;
    float                   mOpacity;

public:
    sf::Color               getFillColor();
    void                    setFillColor(sf::Color color);
    sf::Color               getOutlineColor();
    void                    setOutlineColor(sf::Color color);
    void                    setOpacity(float opacity);
    float                   getOpacity();
    float                   getRadius();
    void                    setValue(int value);
    float                   getTextSize();
    void                    setTextSize(float textSize);   
    void                    setNote(std::string note);
    void                    setSize(float size);

public:
    enum                    NoteAlignment { CENTER, RIGHT };
    void                    setNoteAlignment(NoteAlignment alignment);

protected:
    void                    updateText();
};