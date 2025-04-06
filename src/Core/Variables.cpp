#include <Core/Variables.hpp>

namespace ANIMATION
{
    float Speed = 1.0f;
}

namespace SCREEN
{
    sf::Color BackgroundColor = sf::Color(227, 230, 236); // #E3E6EC
    float Width = 1200;
    float Height = 720;
}

namespace UI
{
    namespace TOOLBOX
    {
        sf::Color Color = sf::Color(198, 203, 214);
        sf::Vector2f Size = sf::Vector2f(300, 540);
        sf::Vector2f Position = sf::Vector2f(0, (SCREEN::Height - Size.y) / 2);
    }
    sf::Color OperationBoxColor = sf::Color(176, 183, 198); // #B0B7C6
}

namespace VIZ    
{
    namespace DS
    {
        sf::Vector2f Size       = sf::Vector2f(SCREEN::Width - UI::TOOLBOX::Size.x, 680); // 900 680
        sf::Vector2f Position   = sf::Vector2f(SCREEN::Width - Size.x, (SCREEN::Height - Size.y) / 2); // 300 10
        sf::Vector2f Center     = sf::Vector2f(Position.x + Size.x / 2, Position.y + Size.y / 2); // 750 360
        float RowSpacing        = Size.y / 8;
    }

    namespace NODE
    {
        float Radius = 16.f;
        sf::Color FillColor     = sf::Color(189, 199, 216);
        sf::Color OutlineColor  = sf::Color(78, 93, 108);
    }

    namespace EDGE
    {
        float Thickness         = 3.f;   
        sf::Color Color         = sf::Color(119, 136, 153);   
    }

    sf::Color TextColor         = sf::Color(44, 62, 80);         
}
    
