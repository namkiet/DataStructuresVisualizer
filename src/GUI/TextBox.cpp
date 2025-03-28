#include "GUI/TextBox.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <iostream>
#include "GUI/ExpandableButton.hpp"
namespace GUI {

const std::string TextBox::mAllowedChars = "0123456789";

TextBox::TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize)
    {
    mSelectOutlineColor = sf::Color(66, 133, 244);
    mDefaultOutlineColor = sf::Color(76, 91, 99);
    InputNum = -1;
    mBox.setSize(size);
    mBox.setPosition(position);
    mBox.setFillColor(sf::Color(31, 42, 47));
    mBox.setOutlineColor(mDefaultOutlineColor);
    mBox.setOutlineThickness(2.f);

    mText.setFont(font);
    mText.setCharacterSize(charSize);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(position.x + 5.f, position.y + (size.y - charSize) / 2.f);
    mText.setString("");
}


void TextBox::select(){
    Component::select();
    mBox.setOutlineColor(mSelectOutlineColor);
}

void TextBox::deselect(){
    Component::deselect();
    mBox.setOutlineColor(mDefaultOutlineColor);
}

void TextBox::reset(){
    InputNum = 0;
    mInput = "";
    mText.setString(mInput);
}

void TextBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed)
    {   

        if (event.key.code == sf::Keyboard::Enter) 
        {   
            if (!mInput.empty())
            {
                InputNum = std::stoi(mInput);
                if (InputNum != 0)
                {
                if(mCallback){
                    mCallback();
                }
                }

                reset();

                deselect();

            }
        }
    }
    else if (isSelected() && event.type == sf::Event::TextEntered) 
    {
        if (event.text.unicode < 128) {
            inputLogic(event.text.unicode);
        }
    } 
    else if (event.type == sf::Event::MouseButtonPressed) 
    {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (mBox.getGlobalBounds().contains(mousePos))
        {
            select();
        } 
        else {
            deselect();
        }
    }
}

void TextBox::inputLogic(sf::Uint32 unicode) {
    char ch = static_cast<char>(unicode);

    if (unicode == 8) { // Backspace
        if (!mInput.empty()) {
            mInput.pop_back();
        }
    }
    else if(mAllowedChars.find(ch) != std::string::npos){
        mInput += ch;
    }
    mText.setString(mInput); 


}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBox, states);
    target.draw(mText, states);
}

void TextBox::setText(const std::string& text) {
    mInput = text;
    mText.setString(mInput);
}

std::string TextBox::getText() const {
    return mInput;
}

void TextBox::setColor(sf::Color color){
    mBox.setFillColor(color);
}

int TextBox::getInputNum(){
    return InputNum;
}
}