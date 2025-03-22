#include <SFML/Graphics.hpp>
#include <GUI/DeliverTextBox.hpp>
#include <GUI/ExpandableButton.hpp>
#include <string>
#include <functional>
#include <iostream>

namespace GUI {
const std::string DeliverTextBox::mAllowedChars = "0123456789";

DeliverTextBox::DeliverTextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize)
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


void DeliverTextBox::setCallback(CallBack callback) {
    mCallBack = std::move(callback);
}

void DeliverTextBox::reset(){
    InputNum = 0;
    mInput = "";
    mText.setString(mInput);
}

void DeliverTextBox::handleEvent(const sf::Event& event) {
    // std::cout<<"DeliverTextBox is handling event"<<std::endl;
    

    // std::cout << "Event detected: " << event.type << std::endl;
    if (event.type == sf::Event::KeyPressed)
    {   
        // std::cout<<"Key pressed is handled "<<std::endl;
        if (event.key.code == sf::Keyboard::Enter) 
        {   
            // std::cout<<"Enter is pressed"<<std::endl;
            if (!mInput.empty())
            {
                InputNum = std::stoi(mInput);
                if (InputNum != 0)
                {
                   ButtonParent->setSubComponentInfo(InputNum);
                }

                reset();
                deselect();
            }
        }
    }
    else if (isSelected() && event.type == sf::Event::TextEntered) 
    {
        // std::cout<<"text entered"<<std::endl;
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

void DeliverTextBox::inputLogic(sf::Uint32 unicode) {
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

    // std::cout<<mInput<<std::endl;

}

void DeliverTextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBox, states);
    target.draw(mText, states);
}

void DeliverTextBox::setText(const std::string& text) {
    mInput = text;
    mText.setString(mInput);
}

std::string DeliverTextBox::getText() const {
    return mInput;
}

void DeliverTextBox::setButtonParent(std::shared_ptr<ExpandableButton> Button){
    ButtonParent = Button;
}

void DeliverTextBox::setCallBack(std::function<void()> func){
    mCallBack = func;
}
}