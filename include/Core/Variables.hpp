#pragma once
#include <SFML/Graphics.hpp>

namespace WINDOW
{
    const float         ScreenWidth = 1200;
    const float         ScreenHeight = 700;
}

namespace THEME
{
    enum Mode           { DarkMode, LightMode};
    inline sf::Color    BackgroundColor;
    inline sf::Color    BoxColor;
    inline sf::Color    TextColor;
}