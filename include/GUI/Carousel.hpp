#pragma once
#include <SFML/Graphics.hpp>
#include <Core/Variables.hpp>
#include <Core/Utility.hpp>
#include <vector>
#include <string>
#include <functional>

class Carousel
{
public:
    struct Item
    {
        enum ItemType           { AVL, HEAP, LINKED_LIST, GRAPH };

        sf::Texture             mTexture;
        sf::Sprite              mSprite;
        sf::CircleShape         mDot;
        ItemType                mType;
        bool                    mIsSelected;
        sf::Vector2f            mInitialPosition;
        std::function<void()>   mCallback;
    
        Item(ItemType type, sf::Texture &texture)
            : mType(type), mIsSelected(false), mCallback(nullptr), mTexture(texture)
        {
            mTexture.setSmooth(true);
    
            mSprite.setTexture(mTexture);
            mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);
    
            mDot.setRadius(5);
            mDot.setFillColor(sf::Color::White);
            mDot.setOrigin(5, 5);
        }
    
        void setDotPosition(sf::Vector2f position)
        {
            mDot.setPosition(position);
        }
    
        void setScale(float scale)
        {
            mSprite.setScale(sf::Vector2f(scale, scale));
        }
    
        float getScale() const
        {
            return mSprite.getScale().x;
        }
    
        void setOffset(sf::Vector2f offset)
        {
            mSprite.setPosition(mInitialPosition + offset);
        }

        float getOpacity()
        {
            return mSprite.getColor().a / 255.f;
        }
    
        void setOpacity(float alpha)
        {
            if (alpha > 1) alpha = 1;
            sf::Color spriteColor = mSprite.getColor();
            spriteColor.a = int(alpha * 255.f);
            mSprite.setColor(spriteColor);

            sf::Color dotColor = mDot.getFillColor();
            dotColor.a = int(alpha * 255.f);
            mDot.setFillColor(dotColor);
        }
        
        bool isClicked(sf::Vector2f mousePos)
        {
            return mSprite.getGlobalBounds().contains(mousePos);
        }
    
        void draw(sf::RenderWindow& window)
        {
            window.draw(mSprite);
            window.draw(mDot);
        }

        void setCallback(std::function<void()> callback)
        {
            mCallback = callback;
        }
    };

private:
    int                     currentIndex = 0;
    float                   targetOffset = 0;
    float                   currentOffset = 0;

private:
    float                   selectedScale = 1;
    float                   defaultScale = 0.75;

private:
    std::vector<Item*>      items;

public:
    void                    pushItem(Item::ItemType type, sf::Texture &texture, std::function<void()> callback);
    void                    next();
    void                    prev();
    void                    update(sf::Time dt);
    void                    draw(sf::RenderWindow& window);
    void                    handleEvent(sf::Event event);
};