#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <StateStack.hpp>

class Graph : public State
{
	public:
								Graph(StateStack& stack, Context context);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
        
	private:
        sf::Text                text;
};