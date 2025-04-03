#include <SceneNode/PseudoCode.hpp>

PseudoCode::PseudoCode(sf::Font& font) : mCurrentStep(-1), mFont(font) {}

void PseudoCode::updateCurrent(sf::Time)
{
    // do nothing
}

void PseudoCode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    for (const auto &text : mTextObjects)
        target.draw(text, states);
}

void PseudoCode::setCode(std::string lines)
{
    float offsetY = 10;
    mTextObjects.clear();
    for (const auto& line : lines) {
        sf::Text text(line, mFont, 14);
        text.setFillColor(sf::Color::White);
        text.setPosition(10, offsetY);
        mTextObjects.push_back(text);
        offsetY += 20;
    }
}

void PseudoCode::setStep(int step) {
    mCurrentStep = step;
    for (size_t i = 0; i < mTextObjects.size(); ++i) {
        mTextObjects[i].setFillColor(i == step ? sf::Color::Yellow : sf::Color::White);
    }
}