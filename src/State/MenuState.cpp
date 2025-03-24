#include <State/MenuState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

MenuState::MenuState(StateStack& stack, Context context): 
	State(stack, context)
{
	mCarousel.pushItem(Carousel::Item::AVL, 		"avl-thumbnail.png", 	[this](){ requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::HEAP, 		"heap-thumbnail.png", 	[this](){ requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::LINKED_LIST, "ll-thumbnail.png", 	[this](){ requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::GRAPH, 		"graph-thumbnail.png", 	[this](){ requestStackPush(States::InAppAVL); });
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	
	mCarousel.draw(window);
}

bool MenuState::update(sf::Time dt)
{
	mCarousel.update(dt);
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Left)
			mCarousel.prev();
		if (event.key.code == sf::Keyboard::Right)
			mCarousel.next();
	}

	mCarousel.handleEvent(event);

	return true;
}