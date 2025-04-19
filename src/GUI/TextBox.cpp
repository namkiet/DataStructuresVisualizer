#include "GUI/TextBox.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <iostream>
#include "GUI/ExpandableButton.hpp"
#include <sstream>      
#include <vector>   
namespace GUI {

const std::string TextBox::mAllowedChars = "0123456789, ";

TextBox::TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, std::string placeholder, InputType inputType) :
    mInputType(inputType), // just 1 number by default
    mCharSize(14),
    mDefaultOutlineColor(sf::Color(255, 255, 255, 50)),
    mSelectOutlineColor(sf::Color::White),
    showMessage(false),
    mAllowedEndline(false)
{
    // mDefaultOutlineColor = UI::TEXTBOX::DefaultBorder;
    // mSelectOutlineColor = UI::TEXTBOX::SelectedBorder;

    InputNum = -1;
    showCursor = false;

    mBox.setSize(size);
    mBox.setPosition(position);
    mBox.setFillColor(sf::Color::Transparent); //UI::TEXTBOX::FillColor);
    mBox.setOutlineColor(mDefaultOutlineColor);
    mBox.setOutlineThickness(2.f);

    mText.setFont(font);
    mText.setCharacterSize(mCharSize);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(position.x + 5.f, position.y + int((size.y - mCharSize) / 2.f));
    mText.setString("_");

    mMessage.setFont(font);
    mMessage.setCharacterSize(14);
    mMessage.setFillColor(sf::Color::White);
    if(mInputType == GUI::TextBox::InputType::Number)
    {
        mMessage.setString("Input a number from 0 to 99");
    }
    else if(mInputType == GUI::TextBox::InputType::VectorNum)
    {
        mMessage.setString("Input 2 number from 0 to 99 seperated by ',' or ' '");
    }

    mPlaceholder.setFont(font);
    mPlaceholder.setCharacterSize(mCharSize);
    mPlaceholder.setFillColor(sf::Color(150, 150, 150));
    mPlaceholder.setPosition(position.x + 5.f, position.y + int((size.y - mCharSize) / 2.f));
    mPlaceholder.setString(placeholder);
}

void TextBox::setPlaceholder(const std::string& text) {
    mPlaceholder.setString(text);
}
sf::FloatRect GUI::TextBox::getGlobalBounds() const {
    return mBox.getGlobalBounds();
}

void GUI::TextBox::setMessage(std::string message){
    mMessage.setString(message);
}

void GUI::TextBox::setTextPosition(sf::Vector2f pos){
    mText.setPosition(pos);
}

void GUI::TextBox::setAllowedEndLine(bool isAllowed){
    mAllowedEndline = isAllowed;
}

void TextBox::select(){
    Component::select();
    mBox.setOutlineColor(mSelectOutlineColor);
    showCursor = true;
    cursorTimer.restart();
}

void TextBox::deselect(){
    Component::deselect();
    mBox.setOutlineColor(mDefaultOutlineColor);
    showCursor = false;
}

void TextBox::reset(){
    InputNum = -1;
    InputNumList.clear();
    mInput = "";
    mText.setString(mInput + "_");
}

void TextBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        if (mBox.getGlobalBounds().contains(mousePos))
        {
            showMessage = true;
            mMessage.setPosition(mousePos.x + 10.f, mousePos.y + 20.f);
        } 
        else 
        {
            showMessage = false;
        }
    }
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        if (event.key.shift && mAllowedEndline) {
            mInput += '\n';
            mText.setString(mInput + "_");
        } else {
            submit();
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
    } else if(mInputType == InputType::Number && isdigit(ch)){
        mInput += ch;
    }
    else if(mInputType == InputType::VectorNum && mAllowedChars .find(ch) != std::string::npos){
        mInput += ch;
    }
    mText.setString(mInput + "_"); 
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBox, states);
    if (mInput.empty()) {
        target.draw(mPlaceholder, states);
    } else {
        target.draw(mText, states);
    }

    if(showMessage){
        target.draw(mMessage,states);
    }

    // if (showCursor && isSelected() && (cursorTimer.getElapsedTime().asSeconds() > 0.5f)) {
    //     sf::RectangleShape cursor(sf::Vector2f(2.f, mText.getCharacterSize()));
    //     cursor.setFillColor(sf::Color::White);
    //     cursor.setPosition(mText.getPosition().x + mText.getGlobalBounds().width + 2, mText.getPosition().y);
    //     target.draw(cursor, states);
    //     if (cursorTimer.getElapsedTime().asSeconds() > 1.f) {
    //         cursorTimer.restart();
    //     }
    // }
}

void TextBox::setText(const std::string& text) {
    mInput = text;
    mText.setString(mInput + "_");
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
void TextBox::submit()
{
    std::cout<<"mInput at textbox"<<mInput<<" "<<std::endl;
    if (!mInput.empty())
    {
        if (mInputType == InputType::Number) {
            try {
                InputNum = std::stoi(mInput);
        
                // if (InputNum < 0 || InputNum > 99) {
                //     reset();
                //     deselect();
                //     return;
                // }
            } catch (const std::invalid_argument& e) {
                std::cout << "Invalid input: not a valid integer.";
                reset();
                deselect();
                return;
            } catch (const std::out_of_range& e) {
                std::cout << "Invalid input: number out of range.";
                reset();
                deselect();
                return;
            }
        }

        else if (mInputType == InputType::VectorNum) {
            std::string temp = mInput;
            mInput.erase(std::remove(mInput.begin(), mInput.end(), '\n'), mInput.end());
            std::replace(temp.begin(), temp.end(), ',', ' ');
            std::stringstream ss(temp);
            std::string tempString;
        
            while (ss >> tempString) {
                try {
                    int num = std::stoi(tempString);
        
                    // if (num < 0 || num > 99) {
                    //     reset();
                    //     deselect();
                    //     std::cout << "Invalid input: number out of range [0,99]." << std::endl;
                    //     return;
                    // }
        
                    InputNumList.push_back(num);
                } catch (const std::invalid_argument& e) {
                    reset();
                    deselect();
                    std::cout << "Invalid input: '" << tempString << "' is not a valid number." << std::endl;
                    return;
                } catch (const std::out_of_range& e) {
                    reset();
                    deselect();
                    std::cout << "Invalid input: number '" << tempString << "' is out of range." << std::endl;
                    return;
                }
            }
        }
        
        if (InputNum != -1 || !InputNumList.empty()) {
            if (mCallback) {
                mCallback();
            }
        }
        reset();
        deselect();
    }
}

std::vector<int> TextBox::getInputNumList(){
    return InputNumList;
}
}