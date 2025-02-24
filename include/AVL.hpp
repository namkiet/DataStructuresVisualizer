#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <StateStack.hpp>
#include <World.hpp>

class AVL : public State
{
	public:
								AVL(StateStack& stack, Context context);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);

	private:
        World                   mWorld;
		
        sf::Text                text;
};