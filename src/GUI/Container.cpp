#include "GUI/Container.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include <iostream>
namespace GUI
{

Container::Container(): mChildren(), mSelectedChild(-1), mActivateChild(-1)
{
}

bool Container::hasSelection() const
{
	return mSelectedChild >= 0;
}

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

	for(const auto& child: mChildren)
		target.draw(*child, states);
}

Component::Ptr Container::getActivateChild(){
    if(mActivateChild != -1){
        return mChildren[mActivateChild];
    }
    return NULL;
}


void Container::pack(Component::Ptr component)
 {
    mChildren.push_back(component);
    // if (!hasSelection() && component->isSelectable())
    //     select(mChildren.size() - 1);
 }

 bool Container::isSelectable() const
 {
    return false;
 }



  void Container::select(std::size_t index) // select duy nhat index
 {
    if (mChildren[index]->isSelectable())
    {
        if (hasSelection() && mSelectedChild == index) return;

        if(hasSelection()) mChildren[mSelectedChild]->deselect();
        
        mChildren[index]->select();
        mSelectedChild = index;
    }
 }
 bool Container::hasActivation(){
    return mActivateChild >= 0;
 }

void Container::ChangeActivateChild(std::size_t index){

    if(hasActivation() && mActivateChild == index) return;

    if(hasActivation()) 
    {
        mChildren[mActivateChild]->deactivate();
        std::cout<<"deselect all"<<std::endl;
    }
    mActivateChild = index;
    if(index != -1){
        mChildren[index]->activate();
    }

    // deselectAll(); // moi lan thay doi activatedChild thi deselect


}


  void Container::handleEvent(const sf::Event& event)
 {
        for(int i = 0; i < mChildren.size();i++)
        {
            mChildren[i]->handleEvent(event);
            if(mChildren[i]->isActive() && i != mActivateChild){ 
                ChangeActivateChild(i);
            }
            else if( i == mActivateChild && !mChildren[i]->isActive()){
                ChangeActivateChild(-1);
            }
            else if(mChildren[i]->isSelectable() && mChildren[i]->isSelected()){
                select(i); // to deselect other
            }
        
        }
 }
 int Container::getSize(){
    return mChildren.size();
}

void Container::makeEmpty(){
    mChildren.clear();
}

void Container::deselectAll(){
    for(auto& child: mChildren){
        if(child->isSelectable()){
            child->deselect();
        }
    }
}

}

