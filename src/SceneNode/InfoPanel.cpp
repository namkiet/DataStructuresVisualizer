#include <SceneNode/InfoPanel.hpp>
#include <sstream>

InfoPanel::InfoPanel(sf::Font &font, sf::Vector2f size):
    mFont(font),
    mSize(size)
{
    mText.setFont(mFont);
    mText.setCharacterSize(16);
    mText.setFillColor(sf::Color::White);
    mText.setPosition(sf::Vector2f(16.f, 16.f));
}

void InfoPanel::setText(const std::string& text)
{
    std::istringstream words("[INFO] " + text);
    std::string word, wrappedText, line;
    sf::Text tempText("", mFont, mText.getCharacterSize());

    while (words >> word) {
        std::string newLine = (line.empty() ? "" : line + " ") + word;
        tempText.setString(newLine);

        if (tempText.getLocalBounds().width > mSize.x - 16)
        {
            wrappedText += (wrappedText.empty() ? "" : "\n") + line;
            line = word; // Bắt đầu dòng mới
        } 
        else 
            line = newLine;
    }
    wrappedText += (wrappedText.empty() ? "" : "\n") + line; // Thêm dòng cuối cùng
    mText.setString(wrappedText);
}

void InfoPanel::setCharacterSize(float size)
{
    mText.setCharacterSize(size);
}

void InfoPanel::updateCurrent(sf::Time dt)
{
    // do nothing    
}

void InfoPanel::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText, states);
}