#pragma once

#include "GUI/Button.hpp"
#include <functional>
namespace GUI
{
class ExpandableButton : public GUI::Button {
public:
    struct subComponentInfo{
        int InfoID; // to determine which kind of action should be called
        int num; // to add/ delete single value;
        std::vector<int> VecNum; // to add/ delete multiple value;
        subComponentInfo(){
            InfoID = -1;
            num = -1; VecNum.resize(0);
        }
    };

    typedef std::shared_ptr<ExpandableButton> Ptr;

    ExpandableButton(sf::Font& fonts, sf::Vector2f Position, std::string text,sf::Vector2f ButtonSize, GUI::Button::ShapeType shapeType = GUI::Button::ShapeType::Rectangle, GUI::Button::ContentType content = GUI::Button::ContentType::Text);

    // void toggleMenu();
    void handleEvent(const sf::Event& event) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void addSubComponent(Component::Ptr component);
    subComponentInfo getSubComponentInfo();
    void setSubComponentInfo(int number, int InfoID);
    void setSubComponentInfo(std::vector<int> vec, int InfoID);
    void resetSubComponentInfo();
    void setFunc(std::function<void()> func);

private:
    subComponentInfo                Info;
    std::vector<GUI::Component::Ptr> mSubComponents;
    bool isExpanded;
    std::function<void()>           mFunc;


    int inputNum;
};

}