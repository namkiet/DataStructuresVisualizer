#include <GUI/InfoPanel.hpp>
#include <sstream>

GUI::InfoPanel::InfoPanel(float width, float height, sf::Vector2f position) {
    mBackground.setSize({width, height});
    mBackground.setFillColor(sf::Color(50, 50, 50, 200));
    mBackground.setOutlineColor(sf::Color::White);
    mBackground.setOutlineThickness(2.f);
    mBackground.setPosition(position);

    mFont.loadFromFile("assets/fonts/jetbrains.ttf");
    mText.setFont(mFont);
    mText.setCharacterSize(20);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(position + sf::Vector2f(10.f, 10.f));
}

// void GUI::InfoPanel::setText(const std::string& newText) {
//     mText.setString(newText);
// }

void GUI::InfoPanel::setText(const std::string& text) {
    std::istringstream words(text);
    std::string word, wrappedText, line;
    sf::Text tempText("", mFont, mText.getCharacterSize());

    while (words >> word) {
        std::string newLine = (line.empty() ? "" : line + " ") + word;
        tempText.setString(newLine);

        if (tempText.getLocalBounds().width > mBackground.getSize().x) {
            wrappedText += (wrappedText.empty() ? "" : "\n") + line;
            line = word; // Bắt đầu dòng mới
        } else {
            line = newLine;
        }
    }
    wrappedText += (wrappedText.empty() ? "" : "\n") + line; // Thêm dòng cuối cùng
    // return wrappedText;

    mText.setString(wrappedText);
}

void GUI::InfoPanel::setCharacterSize(float size)
{
    mText.setCharacterSize(size);
}

void GUI::InfoPanel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mBackground, states);
    target.draw(mText, states);
}