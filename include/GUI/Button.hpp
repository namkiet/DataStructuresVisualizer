#pragma once
#include "Core/ResourceHolder.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Core/ResourceIdentifiers.hpp"
#include<vector>

#include "GUI/Component.hpp"


namespace GUI
{

extern const sf::Vector2f ButtonSize;

class Button: public Component
{
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;

        enum class ShapeType {
            Rectangle,
            Circle
        };
        
        enum class ContentType{
            Text,
            Image
        };
    
    public:
                                Button(sf::Vector2f position, sf::Vector2f buttonSize);
                                Button(sf::Font& font, sf::Vector2f Position, std::string text,sf::Vector2f ButtonSize, ShapeType shapeType = ShapeType::Rectangle, ContentType content = ContentType::Text);

        virtual void			handleEvent(const sf::Event& event);
        void			        draw(sf::RenderTarget& target, sf::RenderStates states) const;

    public:
        void                    setText(std::string text);
        std::string             getText();
        void					setToggle(bool flag); 
        virtual bool			isSelectable() const;
        virtual void			select();
        virtual void			deselect();
        void                    activate() override;
        void                    deactivate() override;
    
    
        void                    setSprite(sf::Sprite sprite);
    protected:
        Callback				mCallback = nullptr;
    public:
        void					setCallback(Callback callback);

    private:
        bool                    mIsToggle;
        sf::Color               mNormalColor;
        sf::Color               mSelectedColor;
        sf::Color               mActivatedColor;
        
        ShapeType               mShapeType;
        ContentType             mContentType;

    protected:
        sf::RectangleShape      mShape;
        sf::CircleShape         mCircle;
        sf::Text				mText;
        sf::Sprite              mSprite;
        // default Rectangle and Text
    
    public:
        void                    setSize(sf::Vector2f size);
        void                    setPosition(sf::Vector2f position);
        void                    setNormalColor(sf::Color color);
        void                    setSelectedColor(sf::Color color);
        void                    setActivatedColor(sf::Color color);
    private:
        void                    updateSpriteSize();
        void                    updateSpritePosition();

};

}