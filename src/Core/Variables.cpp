#include <Core/Variables.hpp>

namespace ANIMATION
{
    float Speed = 1.f;
}

namespace SCREEN
{
    sf::Color BackgroundColor   = sf::Color(76, 44, 132);

    // sf::Texture txt;
    // txt.loadFromFile("assets/images/bg.jpg");
    // sf::Sprite BackgroundImage(txt);
    // BackgroundImage.setTexture(txt);
    
    float Width                 = 1600;
    float Height                = 900;
}

namespace THEME
{
    Theme mTheme = Theme::DesertNight;
}
namespace UI
{
    namespace TOOLBOX
    {
        sf::Color OutlineColor      = sf::Color::White;
        sf::Color FillColor         = sf::Color(0, 0, 0, 100);
        sf::Vector2f Size           = sf::Vector2f(400, 720);
        sf::Vector2f Position       = sf::Vector2f(0, (SCREEN::Height - Size.y) / 2);
    }

    namespace CODEBOX
    {
        sf::Vector2f Position       = TOOLBOX::Position;
        sf::Vector2f Size           = sf::Vector2f(TOOLBOX::Size.x, TOOLBOX::Size.y * 3 / 8);
    }

    namespace INFOBOX
    {
        sf::Vector2f Position       = CODEBOX::Position + sf::Vector2f(0, CODEBOX::Size.y);
        sf::Vector2f Size           = sf::Vector2f(TOOLBOX::Size.x, TOOLBOX::Size.y * 1 / 8);
    }

    namespace OPERATIONBOX
    {
        sf::Vector2f Position       = INFOBOX::Position + sf::Vector2f(0, INFOBOX::Size.y);
        sf::Vector2f Size           = sf::Vector2f(TOOLBOX::Size.x, TOOLBOX::Size.y * 3 / 8);
    }

    namespace CONTROLBOX
    {
        sf::Vector2f Position       = OPERATIONBOX::Position + sf::Vector2f(0, OPERATIONBOX::Size.y);
        sf::Vector2f Size           = sf::Vector2f(TOOLBOX::Size.x, TOOLBOX::Size.y * 1 / 8);
    }

    namespace BUTTON
    {
        sf::Color FillColor         = sf::Color::Transparent;
        sf::Color SelectedColor     = sf::Color(255, 255, 255, 13);
        sf::Color ActivatedColor    = sf::Color(255, 255, 255, 64);
        sf::Color OutlineColor      = sf::Color::Transparent;
    }
}

namespace VIZ    
{
    namespace DS
    {
        sf::Vector2f Size           = sf::Vector2f(SCREEN::Width - UI::TOOLBOX::Size.x, SCREEN::Height - 2 * UI::TOOLBOX::Position.y);
        sf::Vector2f Position       = sf::Vector2f(UI::TOOLBOX::Size.x, UI::TOOLBOX::Position.y);
        sf::Vector2f Center         = sf::Vector2f(Position.x + Size.x / 2, Position.y + Size.y / 2); // 750 360
        float RowSpacing            = Size.y / 8;
    }

    namespace NODE
    {
        float Radius                = 18.f;
        float Thickness             = 2.f;
        sf::Color FillColor         = sf::Color::Transparent;
        sf::Color OutlineColor      = sf::Color::White;
    }

    namespace EDGE
    {
        float Thickness             = 2.f;
        sf::Color Color             = sf::Color::White;   
    }

    sf::Color TextColor             = sf::Color::White;         
}
    
