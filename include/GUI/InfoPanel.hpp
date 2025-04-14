#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace GUI
{
    class InfoPanel : public sf::Drawable {
    public:
        typedef std::unique_ptr<InfoPanel> Ptr;
        InfoPanel(float width, float height, sf::Vector2f position = {10.f, 10.f});

        void setText(const std::string& newText);
        void setCharacterSize(float size);

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape mBackground;
        sf::Text mText;

        sf::Font mFont;
    };
}