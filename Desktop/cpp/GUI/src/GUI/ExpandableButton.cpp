    #include "GUI/ExpandableButton.hpp"
    #include <iostream>
    namespace GUI{

    ExpandableButton::ExpandableButton(sf::Font& fonts, sf::Vector2f Position, std::string text,sf::Vector2f ButtonSize,GUI::Button::ShapeType shapeType, GUI::Button::ContentType content):
    Button(fonts, Position,text,sf::Vector2f(ButtonSize.x,ButtonSize.y),shapeType, content),
    isExpanded(false),
    mSubComponents(),
    Info()
    {

    }

    void ExpandableButton::addSubComponent(Component::Ptr Component){
        mSubComponents.push_back(Component);
    }

    void ExpandableButton::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        mShape.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
    {
        std::cout << "Click " << this->getText() << std::endl;
        activate();
    }

    else if (event.type == sf::Event::MouseMoved)
    {
        sf::Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
        bool hoverNow = mShape.getGlobalBounds().contains(mousePos);

        if (hoverNow && !isActive())
        {
            select();
        }
        else if (!hoverNow && isSelected() && !isActive())
        {
            deselect();
        }
    }

    // 3. Subcomponent event
    if (isActive())
    {
        for (auto& subComponent : mSubComponents)
            {
                subComponent->handleEvent(event);
            }
        if (mCallback)
            mCallback();
    }
}

    void ExpandableButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        Button::draw(target,states);
        if(isActive()){
            for (auto& subComponent : mSubComponents) {
                target.draw(*subComponent, states);
            }
        }
    }

    void ExpandableButton::setSubComponentInfo(int number){
        Info.num = number;
    }
    void ExpandableButton::setSubComponentInfo(std::vector<int> vec){
        Info.VecNum = vec;
    }
    GUI::ExpandableButton::subComponentInfo ExpandableButton::getSubComponentInfo(){
        return Info;
    }
    }