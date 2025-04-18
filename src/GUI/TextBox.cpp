#include "GUI/TextBox.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include <iostream>
#include "GUI/ExpandableButton.hpp"
namespace GUI {

const std::string TextBox::mAllowedChars = "0123456789,";

TextBox::TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, std::string placeholder, InputType inputType) :
    mInputType(inputType), // just 1 number by default
    mCharSize(14),
    mDefaultOutlineColor(sf::Color(255, 255, 255, 50)),
    mSelectOutlineColor(sf::Color::White)
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
    mText.setString("");

    mPlaceholder.setFont(font);
    mPlaceholder.setCharacterSize(mCharSize);
    mPlaceholder.setFillColor(sf::Color(150, 150, 150));
    mPlaceholder.setPosition(position.x + 5.f, position.y + int((size.y - mCharSize) / 2.f));
    mPlaceholder.setString(placeholder);
}

void TextBox::setPlaceholder(const std::string& text) {
    mPlaceholder.setString(text);
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
    mText.setString(mInput);
}

void TextBox::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed)
    {   
        if (event.key.code == sf::Keyboard::Enter) 
        {   
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
    mText.setString(mInput); 
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBox, states);
    if (mInput.empty()) {
        target.draw(mPlaceholder, states);
    } else {
        target.draw(mText, states);
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
void TextBox::submit()
{
    std::cout<<"mInput at textbox"<<mInput<<" "<<std::endl;
    if (!mInput.empty())
    {
        // check if mInput is a number or a list of num
        if(mInputType == InputType::Number){
            try {
                InputNum = std::stoi(mInput);
                assert(InputNum >= 0 && "Number must be greater than or equal to 0.");
            } catch (const std::invalid_argument& e) {
                std::cout<<"Invalid input: not a valid integer.";
            } catch (const std::out_of_range& e) {
                std::cout<<"Invalid input: number out of range.";
            }

        } else if(mInputType == InputType::VectorNum){
            char seperate = ',';
            std::string temp = mInput;
            int seperatorIndex = temp.find_first_of(seperate);
            if(seperatorIndex != std::string::npos) std::cout<<"No seperator found in the input string."<<std::endl; // tam thoi dung chuong trinh neu format sai
            std::string firstNum;
            std::string secondNum;
            if(seperatorIndex != std::string::npos){
            firstNum = temp.substr(0, seperatorIndex);
            secondNum = temp.substr(seperatorIndex + 1, temp.length() - seperatorIndex - 1);
            }
            
            try {
                int num1 = std::stoi(firstNum);
                int num2 = std::stoi(secondNum);
                assert(num1 >= 0 && num2 >= 0 && "Numbers must be greater than 0.");
                InputNumList.push_back(num1);
                InputNumList.push_back(num2);
            } catch (const std::invalid_argument& e) {
                std::cout<<"Invalid input: one of the values is not a number.";
            } catch (const std::out_of_range& e) {
                std::cout<<"Invalid input: one of the values is out of range.";
            }
            

        }

        if (InputNum != -1 || !InputNumList.empty())
        {
            if(mCallback){
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