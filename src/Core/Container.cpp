#include "Core/Container.hpp"

Container::Container(){

}
void Container::handleEvent(const sf::Event& event){
    for(auto& button: ButtonList){
        button->handleEvent(event);
    }
}

 void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(auto& button: ButtonList){
        button->draw(target, states);
    }
 }

 void Container::addButton(Button::Ptr button){
    ButtonList.push_back(button);
 }