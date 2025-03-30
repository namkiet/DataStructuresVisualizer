    #include "GUI/ExpandableButton.hpp"
    #include <iostream>
    namespace GUI{

    ExpandableButton::ExpandableButton(sf::Font& fonts, sf::Vector2f Position, std::string text):
    Button(fonts, Position,text),
    isExpanded(false),
    mSubComponents(),
    Info()
    {

    }

    void ExpandableButton::addSubComponent(Component::Ptr Component){
        mSubComponents.push_back(Component);
    }

    void ExpandableButton::handleEvent(const sf::Event& event){
        if (event.type == sf::Event::MouseButtonPressed && mShape.getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) )
        {
                if(isSelected()) deselect();
                else select();
            }

        else if(isSelected()){
            for(auto& subComponent : mSubComponents){
                subComponent->handleEvent(event);
                // std::cout<<"Event is thrown to "<<this->getText()<<" 's Child"<<std::endl;
            }
            if (mCallback) { mCallback();}
            }
    }

    void ExpandableButton::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        Button::draw(target,states);
        if(isSelected()){
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