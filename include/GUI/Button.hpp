#pragma once
#include "Core/ResourceHolder.hpp"
#include "SFML/Graphics.hpp"
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "Core/ResourceIdentifiers.hpp"
#include<vector>

#include <Core/Utility.hpp>
#include <GUI/Component.hpp>


namespace GUI
{

extern const sf::Vector2f ButtonSize;

class Button: public Component
{
    public:
        typedef std::shared_ptr<Button>		Ptr;
        typedef std::function<void()>		Callback;
    
    public:
                                Button(sf::Vector2f position, sf::Vector2f buttonSize, sf::Texture &texture); // Button with image
                                Button(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font &font, std::string text); // Button with text
                                ~Button();
        virtual void			handleEvent(const sf::Event& event);
        void			        draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // BACK-END
    public:
        std::string             getText();
        void					setToggle(bool flag); 
        virtual bool			isSelectable() const;
        virtual void			select();
        virtual void			deselect();
        void                    activate() override;
        void                    deactivate() override;

    protected:
        Callback				mCallback;

    public:
        void					setCallback(Callback callback);

    // FRONT-END
    protected:
        bool                    mIsToggle;
        sf::Color               mNormalColor;
        sf::Color               mSelectedColor;
        sf::Color               mActivatedColor;

    protected:
        sf::Shape*              mShape;
        sf::Text				mText;
        sf::Sprite              mSprite;
        sf::Vector2f            mSize;
    
    public:
        virtual void            setSize(sf::Vector2f size);
        void                    setPosition(sf::Vector2f position);
    
    // private:
    //     virtual void            updateSpriteSize();
    //     virtual void            updateSpritePosition();

};

}