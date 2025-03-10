#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

void spriteResize(sf::Sprite& sprite, sf::Vector2f size);
void spriteResize(sf::Sprite& sprite, float width, float height);

sf::Vector2f operator*(float a, const sf::Vector2f& v);
sf::Vector2f operator*(const sf::Vector2f& v, float a);

float norm(sf::Vector2f a);
float dist(sf::Vector2f a, sf::Vector2f b);
float angle(sf::Vector2f a, sf::Vector2f b);