#pragma once
#include <SFML/Graphics.hpp>

namespace ANIMATION
{
    extern float Speed;
}

namespace SCREEN
{
    extern sf::Color BackgroundColor;
    // extern sf::Sprite BackgroundImage;
    extern float Width;
    extern float Height;
}

namespace UI
{
    namespace TOOLBOX
    {
        extern sf::Color OutlineColor;
        extern sf::Color FillColor;
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;

    }

    namespace INFOBOX
    {
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;
    }

    namespace CODEBOX
    {
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;
    }

    namespace OPERATIONBOX
    {
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;
    }

    namespace CONTROLBOX
    {
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;
    }
    
    namespace BUTTON
    {
        extern sf::Color FillColor;
        extern sf::Color SelectedColor;
        extern sf::Color ActivatedColor;
        extern sf::Color OutlineColor;
    }
}

namespace VIZ
{
    namespace DS
    {
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;
        extern float RowSpacing;
        extern sf::Vector2f Center;
    }

    namespace NODE
    {
        extern float Radius;
        extern float Thickness;
        extern sf::Color FillColor;
        extern sf::Color OutlineColor;
    }

    namespace EDGE
    {
        extern float Thickness;
        extern sf::Color Color;
    }
    
    extern sf::Color TextColor;
}
