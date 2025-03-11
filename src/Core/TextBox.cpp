#include "Core/TextBox.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <iostream>
namespace GUI {

const std::string TextBox::mAllowedChars = "0123456789";

TextBox::TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize)
     {
    InputNum = -1;
    mBox.setSize(size);
    mBox.setPosition(position);
    mBox.setFillColor(sf::Color::White);
    mBox.setOutlineColor(sf::Color::Black);
    mBox.setOutlineThickness(2.f);

    mText.setFont(font);
    mText.setCharacterSize(charSize);
    mText.setFillColor(sf::Color::Black);
    mText.setPosition(position.x + 5.f, position.y + (size.y - charSize) / 2.f);
    mText.setString("");
}


void TextBox::setCallback(CallBack callback) {
    mCallBack = std::move(callback);
}

void TextBox::reset(){
    InputNum = 0;
    mInput = "";
    mText.setString(mInput);
}


void TextBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Enter) {
        if (!mInput.empty()) {
            InputNum = std::stoi(mInput);
            if (mCallBack && InputNum != 0) {
                mCallBack(InputNum);
            }
            reset();

            deselect();
        }
    }
}
    else if (isSelected() && event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            inputLogic(event.text.unicode);
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (mBox.getGlobalBounds().contains(mousePos)) {
            select();
        } else {
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
}