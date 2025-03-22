#include <SFML/Graphics.hpp>
#include <GUI/Container.hpp>
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



  void Container::select(std::size_t index)
 {
    if (mChildren[index]->isSelectable())
    {
        if (hasSelection() && mSelectedChild == index) return;

        if(hasSelection()) mChildren[mSelectedChild]->deselect();
        
        mChildren[index]->select();
        mSelectedChild = index;
    }
 }
 bool Container::hasMode(){
    return mActivateChild >= 0;
 }

void Container::ChangeActivateChild(std::size_t index){
    if(hasMode() && mActivateChild == index) return;

    if(hasMode()) 
    {
        mChildren[mActivateChild]->deactivate();
    }
    mActivateChild = index;
    mChildren[index]->activate();
}


  void Container::handleEvent(const sf::Event& event)
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
 int Container::getSize(){
    return mChildren.size();
}

void Container::makeEmpty(){
    mChildren.clear();
}
}

