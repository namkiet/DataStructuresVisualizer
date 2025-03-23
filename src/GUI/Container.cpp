#include "GUI/Container.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <iostream>

GUI::Container::Container(): mChildren(), mSelectedChild(-1), mActivateChild(-1)
{
}

bool GUI::Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void GUI::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

	for(const auto& child: mChildren)
		target.draw(*child, states);
}

void GUI::Container::pack(Component::Ptr component)
{
    mChildren.push_back(component);
    // if (!hasSelection() && component->isSelectable())
    //     select(mChildren.size() - 1);
}

bool GUI::Container::isSelectable() const
{
    return false;
}

  void GUI::Container::select(std::size_t index)
 {
    if (mChildren[index]->isSelectable())
    {
        if (hasSelection() && mSelectedChild == index) return;

        if(hasSelection()) mChildren[mSelectedChild]->deselect();
        
        mChildren[index]->select();
        mSelectedChild = index;
    }
 }
 bool GUI::Container::hasActivation(){
    return mActivateChild >= 0;
 }

void GUI::Container::ChangeActivateChild(std::size_t index){
    if(hasActivation() && mActivateChild == index) return;

    if(hasActivation()) 
    {
        mChildren[mActivateChild]->deactivate();
    }
    mActivateChild = index;
    mChildren[index]->activate();
}

void GUI::Container::handleEvent(const sf::Event& event)
{
    for(int i = 0; i < mChildren.size();i++)
    {
        mChildren[i]->handleEvent(event);
        if(mChildren[i]->isActive()){
            ChangeActivateChild(i);
        }
        if(mChildren[i]->isSelectable() && mChildren[i]->isSelected()){
            select(i);
        }
    
    }
}

int GUI::Container::getSize(){
    return mChildren.size();
}

void GUI::Container::makeEmpty(){
    mChildren.clear();
}