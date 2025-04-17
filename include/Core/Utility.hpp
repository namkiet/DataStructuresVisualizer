#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "Core/ResourceIdentifiers.hpp"

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

void spriteResize(sf::Sprite& sprite, sf::Vector2f size);
void spriteResize(sf::Sprite& sprite, float width, float height);

sf::Vector2f operator*(float a, const sf::Vector2f& v);
sf::Vector2f operator*(const sf::Vector2f& v, float a);
sf::Vector2f& operator+=(sf::Vector2f& lhs, const sf::Vector2f& rhs);

float norm(sf::Vector2f a);
float dist(sf::Vector2f a, sf::Vector2f b);
float angle(sf::Vector2f a, sf::Vector2f b);

sf::Vector2f NormalUnitVector(sf::Vector2f vec);
sf::Vector2f Repulsion(float coefficient, sf::Vector2f pos1, sf::Vector2f pos2);
sf::Vector2f Attraction(float coefficient, sf::Vector2f pos1, sf::Vector2f pos2);

sf::Vector2f CenterAttraction(sf::Vector2f pos);

bool isValidNodePosition(sf::Vector2f pos);


void makeValidNodePosition(sf::Vector2f& pos);

Textures::ID getCurrentBackgroundOption();