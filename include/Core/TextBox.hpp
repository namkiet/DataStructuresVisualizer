#pragma once
#include "Core/Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace GUI {

class TextBox : public Component {
public:
    static const std::string mAllowedChars;

    typedef std::function<void(int num)>        CallBack;
    typedef std::shared_ptr<TextBox>            Ptr;

    TextBox(const sf::Font& font, sf::Vector2f position, sf::Vector2f size, unsigned int charSize = 24);

    void handleEvent(const sf::Event& event) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setText(const std::string& text);
    std::string getText() const;

    bool isSelectable() const override { return true; }
    void setCallback(CallBack callback);
    void inputLogic(sf::Uint32 unicode);
    void reset();

private:
    sf::RectangleShape mBox;
    sf::Text mText;
    std::string mInput;
    int InputNum;
    CallBack mCallBack;

 

};

}