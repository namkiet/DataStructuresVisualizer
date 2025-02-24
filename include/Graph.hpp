#ifndef BOOK_Graph_HPP
#define BOOK_Graph_HPP

#include <StateStack.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


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

#endif // BOOK_Graph_HPP