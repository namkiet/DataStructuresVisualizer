#include "Core/Button.hpp"
#include <iostream>
#include "Core/Utility.hpp"
namespace GUI
{


    const sf::Vector2f ButtonSize(150.f, 40.f);

    Button::Button(sf::Font& fonts, sf::Vector2f Position, std::string text)
{
        mIsToggle = true;
        mNormalColor = sf::Color::Blue;
        mSelectedColor = sf::Color(100, 100, 255);

        mShape.setSize(ButtonSize);
        mShape.setPosition(Position);
        mShape.setFillColor(sf::Color::Blue);
        
        mText.setFont(fonts);
        mText.setString(text);
        mText.setCharacterSize(24);
        mText.setFillColor(sf::Color::White);
        centerOrigin(mText);
        mText.setPosition(mShape.getPosition() + ButtonSize/2.f);

    }

    void Button::setCallback(Callback callback){
        mCallback = std::move(callback);
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(mShape,states);
        target.draw(mText, states);
    }


    void Button::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed) 
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (mShape.getGlobalBounds().contains(mousePos))
        {
            
            std::cout << "Click " << this->mText.getString().toAnsiString() << std::endl;
            this->activate();
        }
    }
}

void Button::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool Button::isSelectable() const
{
    return true;
}

void Button::select()
{
	Component::select();
}

void Button::deselect()
{
	Component::deselect();
}

void Button::activate()
{
	Component::activate();

	if (mIsToggle)
		mShape.setFillColor(mSelectedColor);

    mCallback();

	if (!mIsToggle)
		deactivate();
}

void Button::deactivate()
{
	Component::deactivate();

	if (mIsToggle)
	{
        mShape.setFillColor(mNormalColor);
	}
}



}