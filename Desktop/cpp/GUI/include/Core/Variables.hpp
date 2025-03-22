#pragma once
#include <SFML/Graphics.hpp>

namespace THEME 
{
    enum Mode           { DARK_MODE, LIGHT_MODE };

    Mode                ThemeMode;
    sf::Color           BackgroundColor;
    sf::Color           ToolBoxColor;
    sf::Color           OperationBoxColor;
    sf::Color           TextColor;
    sf::Font            TextFont;

    void ApplyTheme()
    {
        if (ThemeMode == DARK_MODE)
        {
            BackgroundColor     = sf::Color(37, 50, 56);
            ToolBoxColor        = sf::Color(31, 42, 47);
            OperationBoxColor   = sf::Color(56, 71, 79);
            TextColor           = sf::Color::White;
        }
        else
        {
            BackgroundColor     = sf::Color(220, 225, 230);  
            ToolBoxColor        = sf::Color(200, 205, 210);  
            OperationBoxColor   = sf::Color(180, 190, 200);  
            TextColor           = sf::Color(30, 30, 30);  
        }
    }

    void SetTheme(Mode mode)
    {
        ThemeMode = mode;
        ApplyTheme();
    }
}