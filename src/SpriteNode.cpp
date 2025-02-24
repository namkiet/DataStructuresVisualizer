#include <SpriteNode.hpp>
#include <Utility.hpp>

SpriteNode::SpriteNode(const sf::Texture& texture): mSprite(texture)
{
}

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& rect): mSprite(texture, rect)
{
}

void SpriteNode::setSize(sf::Vector2f size)
{
    spriteResize(mSprite, size);
}

void SpriteNode::setSize(float width, float height)
{
    spriteResize(mSprite, width, height);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}