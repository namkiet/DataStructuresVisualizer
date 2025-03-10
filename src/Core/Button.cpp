#include "Core/Button.hpp"
#include <iostream>
#include "Core/Utility.hpp"
    
    const sf::Vector2f ButtonSize(150.f, 100.f);

    Button::Button(const FontHolder& fonts, const TextureHolder& textures, sf::Vector2f Position)
    {
        mSprite.setTexture(textures.get(Textures::ID::Button));
        action = User::Action::None;
        const sf::Font& font = fonts.get(Fonts::Main); 

        mText.setFont(font);
        mText.setCharacterSize(24);
        mText.setFillColor(sf::Color::Black);

        spriteResize(mSprite, ButtonSize); 
        mSprite.setPosition(Position);

        centerOrigin(mText);
        sf::Vector2f center(Position.x + ButtonSize.x * 0.25f, Position.y + ButtonSize.y*0.3f);
        mText.setPosition(center);

    }
    void Button::setText(const std::string& text){
        mText.setString(text);
    }

    void Button::setAction(User::Action action){
        this->action = action;
    }

    User::Action Button::getAction()
    {
        return action;
    }
    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mSprite,states);
        target.draw(mText, states); // draw text
    }


    void Button::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) 
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (mSprite.getGlobalBounds().contains(mousePos))
        {
            std::cout << "Click " << this->mText.getString().toAnsiString() << std::endl;

            //send action to User to create Command

        }
    }
}
