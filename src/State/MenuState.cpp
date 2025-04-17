#include <State/MenuState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

MenuState::MenuState(StateStack& stack, Context context): 
	State(stack, context)
{
	mBackground.setTexture(context.textures->get(Textures::AppBackground));
	sf::Color color = mBackground.getColor();
	color.a = 0;
	mBackground.setColor(sf::Color(100, 100, 100));

	mCarousel.pushItem(Carousel::Item::AVL, 		context.textures->get(Textures::AVLThumbnail), 	[this](){ requestStackPop(); requestStackPush(States::InAppAVL); });
	mCarousel.pushItem(Carousel::Item::HEAP, 		context.textures->get(Textures::HeapThumbnail), 	[this](){ requestStackPop(); requestStackPush(States::InAppHeap); });
	mCarousel.pushItem(Carousel::Item::LINKED_LIST, context.textures->get(Textures::LLThumbnail), 	[this](){ requestStackPop(); requestStackPush(States::InAppLinkedList); });
	mCarousel.pushItem(Carousel::Item::GRAPH, 		context.textures->get(Textures::GraphThumbnail), 	[this](){ requestStackPop(); requestStackPush(States::InAppGraph); });
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());

	window.draw(mBackground);
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


		if (event.key.code == sf::Keyboard::A)
		{
            requestStackPop();
			requestStackPush(States::Settings);
		}
	}
	mCarousel.handleEvent(event);
	return true;
}