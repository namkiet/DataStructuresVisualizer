#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PseudoCode {
public:
                                PseudoCode(sf::Font& font, float x, float y);
    void                        setStep(int step);
    void                        draw(sf::RenderWindow& window);

private:
    std::vector<std::string>    lines;
    std::vector<sf::Text>       textObjects;
    sf::RectangleShape          background;
    int                         currentStep;
};
