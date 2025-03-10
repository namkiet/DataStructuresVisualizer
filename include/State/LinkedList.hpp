#ifndef BOOK_LinkedList_HPP
#define BOOK_LinkedList_HPP

#include <State/StateStack.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "Core/World.hpp"

class LinkedList : public State
{
	public:
								LinkedList(StateStack& stack, Context context);
		virtual void			draw();
		virtual bool			update(sf::Time dt);
		virtual bool			handleEvent(const sf::Event& event);
        
	private:
        World 					mWorld;
		User					mUser;

		

};

#endif // BOOK_LinkedList_HPP