#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>

class InfoPanel : public SceneNode
{
public:
    typedef std::unique_ptr<InfoPanel>  Ptr;
                                        InfoPanel(sf::Font& font, sf::Vector2f size);

    void                                setText(std::string newText);
    void                                setCharacterSize(float size);
    std::string                         getText() const;

private:
    virtual void                        updateCurrent(sf::Time dt) override;
    virtual void                        drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Text                            mText;
    sf::Font                            mFont;
    sf::Vector2f                        mSize;
    std::string                         mContent;
};