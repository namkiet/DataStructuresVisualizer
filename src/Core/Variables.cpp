#include <Core/Variables.hpp>

namespace ANIMATION
{
    float Speed = 1.0f;
}

namespace SCREEN
{
    sf::Color BackgroundColor   = sf::Color::Black;
    float Width                 = 1200;
    float Height                = 720;
}

namespace UI
{
    namespace TOOLBOX
    {
        sf::Color OutlineColor  = sf::Color::White;
        sf::Color FillColor     = sf::Color(0, 0, 0, 100);
        sf::Vector2f Size       = sf::Vector2f(300, 540);
        sf::Vector2f Position   = sf::Vector2f(0, (SCREEN::Height - Size.y) / 2);
    }

    namespace BUTTON
    {
        sf::Color FillColor     = sf::Color::Transparent;
        sf::Color OutlineColor  = sf::Color::White;
    }
}

namespace VIZ
{
    namespace DS
    {
        sf::Vector2f Size       = sf::Vector2f(SCREEN::Width - UI::TOOLBOX::Size.x, 700);
        sf::Vector2f Position   = sf::Vector2f(SCREEN::Width - Size.x, (SCREEN::Height - Size.y) / 2);
        float RowSpacing        = Size.y / 8;
    }

    namespace NODE
    {
        float Radius = 16.f;
        float Thickness = 2.f;
        sf::Color FillColor     = sf::Color::Transparent;
        sf::Color OutlineColor  = sf::Color::White;
    }

    namespace EDGE
    {
        float Thickness         = 2.f;
        sf::Color Color         = sf::Color::White;   
    }

    sf::Color TextColor         = sf::Color::White;         
}
    
