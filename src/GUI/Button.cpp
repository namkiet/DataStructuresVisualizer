#include "GUI/Button.hpp"
#include <iostream>
#include "Core/Utility.hpp"
#include <cmath>
#include "GUI/Component.hpp"
namespace GUI
{

Button::Button(sf::Font& font, sf::Vector2f Position, std::string text, sf::Vector2f ButtonSize,ShapeType shapeType, ContentType content) : mShapeType(shapeType)
{
    mShapeType = shapeType;
    mContentType = content;
    mIsToggle = true;
    mNormalColor = sf::Color(56, 71, 79);
    mSelectedColor = sf::Color(76, 91, 99);
    mActivatedColor = sf::Color(96, 121, 129);


    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(20);
    mText.setFillColor(sf::Color::White);
    if(mShapeType == ShapeType::Rectangle)
    {
        mShape.setSize(ButtonSize);
        mShape.setPosition(Position);
        mShape.setFillColor(mNormalColor);
        if(content == ContentType::Text)
        {
            centerOrigin(mText);
            mText.setPosition(mShape.getPosition() + ButtonSize / 2.f);
        }
    }
    else if(mShapeType == ShapeType::Circle)
    {
        float radius = ButtonSize.x / 2.f;
        mCircle.setRadius(radius);
        mCircle.setPosition(Position);
        mCircle.setFillColor(mNormalColor);
        if(content == ContentType::Text)
        {
            centerOrigin(mText);
            mText.setPosition(mCircle.getPosition() + sf::Vector2f(radius, radius));
        }

    }
}

void Button::setSprite(sf::Sprite sprite)
{
    mSprite = sprite;
    centerOrigin(mSprite);
    if(mShapeType == ShapeType::Rectangle)
    {   
        sf::Vector2f size = mShape.getSize() / std::sqrt(2.f);
        spriteResize(mSprite, size);
        mSprite.setPosition(mShape.getPosition() + mShape.getSize() / 2.f);
    }
    else if(mShapeType == ShapeType::Circle)
    {
        sf::Vector2f size = sf::Vector2f(mCircle.getRadius() * 2.f / std::sqrt(2.f), mCircle.getRadius() * 2.f/ std::sqrt(2.f));
        spriteResize(mSprite, size);
        mSprite.setPosition(mCircle.getPosition() + sf::Vector2f(mCircle.getRadius(), mCircle.getRadius()));
    }
}

std::string Button::getText(){
    return mText.getString().toAnsiString();
}

// void Button::setCallback(Callback callback){
//     mCallback = std::move(callback);
// }

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(mShapeType == ShapeType::Rectangle)
        target.draw(mShape, states);
    else if(mShapeType == ShapeType::Circle)
        target.draw(mCircle, states);

    if(mContentType == ContentType::Text)
        target.draw(mText, states);
    else if(mContentType == ContentType::Image)
        {
            target.draw(mSprite, states);
    }
}

void Button::handleEvent(const sf::Event& event)
{
if (event.type == sf::Event::MouseButtonPressed) 
{
    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

    if (mShapeType == ShapeType::Rectangle && mShape.getGlobalBounds().contains(mousePos)
        || mShapeType == ShapeType::Circle && mCircle.getGlobalBounds().contains(mousePos))
    {
        
        std::cout << "Click " << this->mText.getString().toAnsiString() << std::endl;
        this->activate();
    }
}
else if(event.type == sf::Event::MouseMoved){
    sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
    bool hoverNow = false;
    if(mShapeType == ShapeType::Rectangle)
        hoverNow = mShape.getGlobalBounds().contains(mousePos);
    else if(mShapeType == ShapeType::Circle)
        hoverNow = mCircle.getGlobalBounds().contains(mousePos);
    if(hoverNow && !isActive())
    {
        select();
    }
    else if(!hoverNow && isSelected() && !isActive())
    {
        deselect();
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


// selected and activated are not allowed to appear at the same time

void Button::select() // khong the select 1 component dang active
{
    if(isActive()) return;
	Component::select();
    if (mIsToggle){
        std::cout<<"Change to selected color"<<std::endl;
        if(mShapeType == ShapeType::Rectangle)
            mShape.setFillColor(mSelectedColor);
        else if(mShapeType == ShapeType::Circle)
            mCircle.setFillColor(mSelectedColor);
    }
    if(!mIsToggle)
        deselect();
}

void Button::deselect() // khong the deselect 1 component dang active
{
	Component::deselect();
    if(mIsToggle && !isActive())
    {
        if(mShapeType == ShapeType::Rectangle)
            mShape.setFillColor(mNormalColor);
        else if(mShapeType == ShapeType::Circle)
            mCircle.setFillColor(mNormalColor);
    }
}

void Button::activate()
{
    deselect();
	Component::activate();
	if (mIsToggle)
		{
            if(mShapeType == ShapeType::Rectangle)
                mShape.setFillColor(mActivatedColor);
            else if(mShapeType == ShapeType::Circle)
                mCircle.setFillColor(mActivatedColor);
        }
    if (mCallback) mCallback();
	if (!mIsToggle)
		deactivate();
}


void Button::deactivate()
{   
	Component::deactivate();
	if (mIsToggle)
	{
        if(mShapeType == ShapeType::Rectangle)
            mShape.setFillColor(mSelectedColor);
        else if(mShapeType == ShapeType::Circle)
            mCircle.setFillColor(mSelectedColor);
    }
}


}