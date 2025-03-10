#pragma once
#include "Core/Button.hpp"
#include <vector>

class Container: public sf::Drawable
{
    private:
        std::vector<Button::Ptr> ButtonList;
    public:
        Container();
        void handleEvent(const sf::Event& event);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        void addButton(Button::Ptr button);
};
