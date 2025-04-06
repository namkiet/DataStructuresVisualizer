#pragma once
#include <SFML/Graphics.hpp>

namespace ANIMATION
{
    extern float Speed;
}

namespace SCREEN
{
    extern sf::Color BackgroundColor;
    extern float Width;
    extern float Height;
}

namespace UI
{
    namespace TOOLBOX
    {
        extern sf::Color Color;
        extern sf::Vector2f Size;
        extern sf::Vector2f Position;

    }
    extern sf::Color OperationBoxColor;
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
