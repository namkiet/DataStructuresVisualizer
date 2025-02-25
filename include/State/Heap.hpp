#ifndef BOOK_Heap_HPP
#define BOOK_Heap_HPP

#include <State/StateStack.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class Heap : public State
{
	public:
								Heap(StateStack& stack, Context context);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
        
	private:
        sf::Text                text;
};

#endif // BOOK_Heap_HPP