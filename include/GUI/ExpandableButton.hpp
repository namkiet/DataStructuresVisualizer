#pragma once

#include <GUI/RectangleButton.hpp>
namespace GUI
{
class ExpandableButton : public Button {
public:
    struct SubComponentInfo
    {
        int num;
        std::vector<int> VecNum;
        SubComponentInfo()
        {
            num = -1; 
            VecNum.resize(0);
        }
    };

    typedef std::shared_ptr<ExpandableButton> Ptr;

                                        ExpandableButton(sf::Vector2f position, sf::Vector2f buttonSize, sf::Font &font, std::string text);
    void                                handleEvent(const sf::Event& event) override;
    void                                draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void                                addSubComponent(Component::Ptr component);
    SubComponentInfo                    getSubComponentInfo();
    void                                setSubComponentInfo(int number);
    void                                setSubComponentInfo(std::vector<int> vec);

private:
    SubComponentInfo                    Info;
    std::vector<GUI::Component::Ptr>    mSubComponents;
    bool                                isExpanded;
    int                                 inputNum;
};

}