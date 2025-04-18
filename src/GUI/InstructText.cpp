#include<GUI/InstructText.hpp>
#include<iostream>
namespace GUI
{
InstructText::InstructText() {
    mInstruct = std::make_shared<sf::Text>();
    mInstruct->setString("");
    hasInfo = false;
}
void InstructText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // std::cout<<"JUmp to draw instruction ok"<<std::endl;
    if(hasInfo){
        std::cout<<"It has info and now draw"<<std::endl;
        target.draw(*mInstruct, states);
    }
}
}