#pragma once

namespace ANIMATION
{
    inline float                Speed                   = 10.0f;
} // namespace ANIMATION

namespace SCREEN
{
    inline float                Width                   = 1200;
    inline float                Height                  = 720;
}

namespace LAYOUT
{
    inline float                ScreenWidth             = 1200;
    inline float                ScreenHeight            = 720;

    namespace UI
    {
        inline float            Width                   = 100;
        inline float            Height                  = 100;
    }
    

    namespace DS
    {
        inline float            Width                   = 1000;
        inline float            Height                  = 700;
        inline sf::Vector2f     Position                = sf::Vector2f(ScreenWidth - Width, (ScreenHeight - Height) / 2);
        inline float            RowSpacing              = Height / 8; // Max of 6 rows

        inline float            NodeRadius              = 16.f;
        inline float            EdgeThickness           = 2.f;
    }
}

namespace COLOR
{
    namespace DS
    {
        inline sf::Color        NodeFillColor           = sf::Color::White;
        inline sf::Color        NodeOutlineColor        = sf::Color::Black;
        inline sf::Color        NodeTextColor           = sf::Color::Black;
    }
}