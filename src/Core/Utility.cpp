#include <Core/Utility.hpp>

void centerOrigin(sf::Shape& shape)
{
	sf::FloatRect bounds = shape.getLocalBounds();
	shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Sprite& sprite)
{
	sf::FloatRect bounds = sprite.getLocalBounds();
	sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void centerOrigin(sf::Text& text)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
}

void spriteResize(sf::Sprite& sprite, sf::Vector2f size)
{
	float curWidth = sprite.getGlobalBounds().width;
	float curHeight = sprite.getGlobalBounds().height;
	sprite.setScale(sf::Vector2f(size.x / curWidth, size.y / curHeight));
}

void spriteResize(sf::Sprite& sprite, float width, float height)
{
	float curWidth = sprite.getGlobalBounds().width;
	float curHeight = sprite.getGlobalBounds().height;
	sprite.setScale(sf::Vector2f(width / curWidth, height / curHeight));
}

sf::Vector2f operator*(float a, const sf::Vector2f& v) {
    return sf::Vector2f(a * v.x, a * v.y);
}

sf::Vector2f operator*(const sf::Vector2f& v, float a) {
    return sf::Vector2f(v.x * a, v.y * a);
}

float norm(sf::Vector2f a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

float dist(sf::Vector2f a, sf::Vector2f b)
{
	return norm(a - b);
}

float angle(sf::Vector2f a, sf::Vector2f b) 
{
	sf::Vector2f vec = b - a;
    return std::atan2(vec.y, vec.x);
}