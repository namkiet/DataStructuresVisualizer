#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>

class SpriteNode : public SceneNode
{
public:
    explicit                SpriteNode(const sf::Texture& texture);
                            SpriteNode(const sf::Texture& texture, const sf::IntRect& rect);

private:
    sf::Sprite              mSprite;

public:
    void                    setSize(sf::Vector2f size);
    void                    setSize(float width, float height);
    sf::FloatRect           getGlobalBounds() const;


protected:
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
};