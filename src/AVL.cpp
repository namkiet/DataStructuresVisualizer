#include <AVL.hpp>
#include <ResourceHolder.hpp>
#include <Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

#include <iostream>

AVL::AVL(StateStack& stack, Context context): 
    State(stack, context),
    mWorld(*context.window, *context.textures)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	text.setFont(font);
	text.setString("This is AVL");
	centerOrigin(text);
	text.setPosition(context.window->getView().getSize() / 2.f);
}

void AVL::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
    window.draw(text);
    // window.draw()
    mWorld.draw();
}

bool AVL::update(sf::Time dt)
{
    mWorld.update(dt);
	return true;
}

bool AVL::handleEvent(const sf::Event& event)
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