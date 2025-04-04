#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <vector>
#include <string>

class PseudoCode: public SceneNode 
{
public:
    typedef std::unique_ptr<PseudoCode> Ptr;
                                        PseudoCode(sf::Font& font);
    void                                setStep(int step);
    void                                setCode(std::vector<std::string> lines);

private:
    virtual void                        updateCurrent(sf::Time dt) override;
    virtual void                        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Font                            mFont;
    std::vector<std::string>            mLines;
    std::vector<sf::Text>               mTextObjects;
    int                                 mCurrentStep;
};
