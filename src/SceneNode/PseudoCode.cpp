#include <SceneNode/PseudoCode.hpp>

PseudoCode::PseudoCode(sf::Font& font, float x, float y) : currentStep(-1), mFont(font)
{
    lines = {
        "insert node normally",
        "update height",
        "check balance:",
        "    if LL -> rotateRight",
        "    if RR -> rotateLeft",
        "    if LR -> rotateLeft on left child, then rotateRight",
        "    if RL -> rotateRight on right child, then rotateLeft"
    };

    background.setSize(sf::Vector2f(500, 200));
    background.setPosition(x, y);
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(2);

    float offsetY = 10;
    for (const auto& line : lines) 
    {
        sf::Text text(line, font, 14);
        text.setFillColor(sf::Color::White);
        text.setPosition(x + 10, y + offsetY);
        textObjects.push_back(text);
        offsetY += 20;
    }
}

void PseudoCode::setStep(int step) 
{
    currentStep = step;
    for (size_t i = 0; i < textObjects.size(); ++i)
    {
        textObjects[i].setFillColor(i == step ? sf::Color::Yellow : sf::Color::White);
    }
}

void PseudoCode::updateCurrent(sf::Time dt)
{
}

void PseudoCode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const 
{
    target.draw(background, states);
    // window.draw(background);
    for (const auto& text : textObjects) 
    {
        // window.draw(text);
        target.draw(text, states);
    }
}
