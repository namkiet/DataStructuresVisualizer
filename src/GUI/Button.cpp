#include "GUI/Button.hpp"
#include <iostream>
#include <cmath>

GUI::Button::Button(sf::Vector2f position, sf::Vector2f buttonSize, sf::Texture &texture):
    mSize(buttonSize),
    mShape(new sf::RectangleShape(buttonSize))
{
    mIsToggle = true;
    mNormalColor = sf::Color(56, 71, 79);
    mSelectedColor = sf::Color(76, 91, 99);
    mActivatedColor = sf::Color(96, 121, 129);

    mShape->setPosition(position);
    mShape->setFillColor(mNormalColor);
    mSprite.setTexture(texture);
    mSprite.setPosition(position);
    spriteResize(mSprite, buttonSize);
}

GUI::Button::~Button()
{
    delete mShape;
}

GUI::Button::Button(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font &font, std::string text):
    mSize(buttonSize),
    mShape(new sf::RectangleShape(buttonSize))
{
    mIsToggle = true;
    mNormalColor = sf::Color(56, 71, 79);
    mSelectedColor = sf::Color(76, 91, 99);
    mActivatedColor = sf::Color(96, 121, 129);

    mShape->setPosition(position);
    mShape->setFillColor(mNormalColor);

    mText.setFont(font);
    mText.setString(text);
    mText.setCharacterSize(20);
    mText.setFillColor(sf::Color::White);
    centerOrigin(mText);
}

void GUI::Button::setSize(sf::Vector2f size)
{
    mSize = size;
    spriteResize(mSprite, size);
}   

void GUI::Button::setPosition(sf::Vector2f position)
{
    mShape->setPosition(position);
    mSprite.setPosition(position);
    mText.setPosition(position);
}   

std::string GUI::Button::getText(){
    return mText.getString().toAnsiString();
}

void GUI::Button::setCallback(Callback callback){
    mCallback = std::move(callback);
}

void GUI::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{   
    target.draw(*mShape, states);
    target.draw(mText, states);
    target.draw(mSprite, states);
}

void GUI::Button::handleEvent(const sf::Event& event)
{

    sf::FloatRect bounds = mShape->getGlobalBounds();

    if (event.type == sf::Event::MouseButtonPressed) 
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        std::cerr << mousePos.x << " " << mousePos.y << " " << mShape->getGlobalBounds().top << "\n";

        if (mShape->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            std::cerr << bounds.left << " " << bounds.top << " " << bounds.width << " " << bounds.height << "\n";
            // std::cerr << "Click " << this->mText.getString().toAnsiString() << std::endl;
            this->activate();
        }
    }
    else if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        bool isHovered = mShape->getGlobalBounds().contains(mousePos);

        if (isHovered) std::cerr << "2134\n";

        if (isHovered && !isActive())
            select();
        else if (!isHovered && isSelected() && !isActive())
            deselect();
    }
}

void GUI::Button::setToggle(bool flag)
{
	mIsToggle = flag;
}

bool GUI::Button::isSelectable() const
{
    return true;
}

void GUI::Button::select()
{
	Component::select();
    std::cerr << "Select \n";
    if (mIsToggle){
        std::cerr << "OK\n";
        mShape->setFillColor(mSelectedColor);
    }

    if (!mIsToggle)
        deselect();
}

void GUI::Button::deselect()
{
	Component::deselect();
    if(mIsToggle && !isActive())
    {
        mShape->setFillColor(mNormalColor);
    }
}

void GUI::Button::activate()
{
	Component::activate();
	if (mIsToggle)
        mShape->setFillColor(mActivatedColor);

    mCallback();
	if (!mIsToggle)
		deactivate();
}


void GUI::Button::deactivate()
{   
	Component::deactivate();
	if (mIsToggle)
        mShape->setFillColor(mSelectedColor);
}