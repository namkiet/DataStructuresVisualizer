#include <Core/Utility.hpp>
#include <iostream>
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

sf::Vector2f operator *(float a, const sf::Vector2f& v) {
    return sf::Vector2f(a * v.x, a * v.y);
}

sf::Vector2f operator *(const sf::Vector2f& v, float a) {
    return sf::Vector2f(v.x * a, v.y * a);
}

sf::Vector2f& operator+=(sf::Vector2f& lhs, const sf::Vector2f& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
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

sf::Vector2f NormalUnitVector(sf::Vector2f vec){
	return sf::Vector2f(vec.y / norm(vec), vec.x * (-1) / norm(vec));
	
}

sf::Vector2f Repulsion(float coefficient, sf::Vector2f pos1, sf::Vector2f pos2)
{
	float dist = norm(pos1 - pos2);
	if (dist == 0) return sf::Vector2f(0, 0);
	return (pos1 - pos2)/norm(pos1-pos2) * (coefficient / (dist * dist));
} // repulsion of pos 1 acting on pos 2

sf::Vector2f Attraction(float coefficient, sf::Vector2f pos1, sf::Vector2f pos2)
{
	// std::cout<<"Jump to attraction ok"<<std::endl;
	float dist = norm(pos1 - pos2);
	if (dist == 0) return sf::Vector2f(0, 0);
	return (pos2 - pos1)/norm(pos1-pos2) * (coefficient / (dist * dist));
} // attraction of pos1 acting on pos2