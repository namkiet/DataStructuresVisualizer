#pragma once
#include <SFML/Graphics.hpp>
#include <GUI/Button.hpp>
#include <Core/Variables.hpp>
#include <vector>
#include <string>

class Carousel
{
public:
    struct Item
    {
        enum ItemType       {AVL, HEAP, LINKED_LIST, GRAPH};

        sf::Texture         mTexture;
        sf::Sprite          mSprite;
        GUI::Button         mButton;
        sf::CircleShape     mDot;
        ItemType            mType;
        bool                mIsSelected;
        sf::Vector2f        mInitialPosition;
    
        Item(ItemType type, std::string imageName)
            : mType(type), mIsSelected(false), mButton(GUI::Button(sf::Vector2f(100, 100), sf::Vector2f(300, 300)))
        {
            if (!mTexture.loadFromFile("assets/images/" + imageName))
                exit(0);
    
            mSprite.setTexture(mTexture);
            mSprite.setOrigin(mTexture.getSize().x / 2, mTexture.getSize().y / 2);
    
            mDot.setRadius(5);
            mDot.setFillColor(sf::Color::White);
            mDot.setOrigin(5, 5);

            mButton.setSprite(mSprite);
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
            mButton.setPosition(mInitialPosition + offset);
        }
    
        void setOpacity(float alpha)
        {
            sf::Color color = mSprite.getColor();
            color.a = alpha * 255.f;
            mSprite.setColor(color);
        }
    
        void setDotOpacity(float alpha)
        {
            sf::Color color = mDot.getFillColor();
            color.a = alpha * 255.f;
            mDot.setFillColor(color);
        }
    
        bool isClicked(sf::Vector2f mousePos)
        {
            return mSprite.getGlobalBounds().contains(mousePos);
        }
    
        void draw(sf::RenderWindow& window)
        {
            window.draw(mButton);
            window.draw(mDot);
        }

        void setCallback(GUI::Button::Callback callback)
        {
            mButton.setCallback(callback);
        }

        void handleEvent(sf::Event event)
        {
            mButton.handleEvent(event);
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
    void                    pushItem(Item::ItemType type, std::string imageName, GUI::Button::Callback callback);
    void                    next();
    void                    prev();
    void                    update(sf::Time dt);
    void                    checkClick(sf::Vector2f mousePos);
    void                    draw(sf::RenderWindow& window);
    void                    handleEvent(sf::Event event);
};