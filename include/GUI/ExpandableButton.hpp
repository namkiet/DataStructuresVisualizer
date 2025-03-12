#pragma once

#include "GUI/Button.hpp"
namespace GUI
{
class ExpandableButton : public GUI::Button {
public:
    typedef std::shared_ptr<ExpandableButton> Ptr;

    ExpandableButton(sf::Font& fonts, sf::Vector2f Position, std::string text);

    // void toggleMenu();
    void handleEvent(const sf::Event& event) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void addSubComponent(Component::Ptr component);

private:
    std::vector<GUI::Component::Ptr> mSubComponents;
    bool isExpanded;
};

}