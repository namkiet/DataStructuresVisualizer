#include <State/Heap.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

Heap::Heap(StateStack& stack, Context context): 
    State(stack, context)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	text.setFont(font);
	text.setString("This is Heap");
	centerOrigin(text);
	text.setPosition(context.window->getView().getSize() / 2.f);
}

void Heap::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());
    window.draw(text);
}

bool Heap::update(sf::Time)
{
	return true;
}

bool Heap::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {   
            requestStackPop();
            requestStackPush(States::Menu);
        }
    }

	return true;
}