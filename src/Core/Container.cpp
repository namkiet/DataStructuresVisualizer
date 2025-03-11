#include "Core/Container.hpp"

#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
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
    if (!hasSelection() && component->isSelectable())
        select(mChildren.size() - 1);
 }

 bool Container::isSelectable() const
 {
    return false;
 }



  void Container::select(std::size_t index)
 {
    if (mChildren[index]->isSelectable())
    {
        if (hasSelection())
            mChildren[mSelectedChild]->deselect();
        mChildren[index]->select();
        mSelectedChild = index;
    }
 }
 bool Container::hasMode(){
    return mActivateChild >= 0;
 }

void Container::ChangeOption(std::size_t index){
    if(hasMode()) mChildren[mActivateChild]->deactivate();
    mActivateChild = index;
}


  void Container::handleEvent(const sf::Event& event)
 {
    if (event.type == sf::Event::MouseButtonPressed){
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        for(int i = 0; i < mChildren.size();i++){
            mChildren[i]->handleEvent(event);
            if(mChildren[i]->isActive()){
                ChangeOption(i);
            }
        }
    }
 }
 int Container::getSize(){
    return mChildren.size();
}
}

