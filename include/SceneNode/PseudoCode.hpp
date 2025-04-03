#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <vector>
#include <string>

class PseudoCode: public SceneNode
{
public:
                                PseudoCode(sf::Font& font, float x, float y);
    void                        setStep(int step);

private:
    virtual void                updateCurrent(sf::Time dt);
    virtual void                drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    std::vector<std::string>    lines;
    std::vector<sf::Text>       textObjects;
    sf::RectangleShape          background;
    int                         currentStep;

    sf::Font                    mFont;
};
