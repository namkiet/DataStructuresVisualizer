#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>
#include <GUI/Carousel.hpp>


class MenuState : public State
{
public:
							MenuState(StateStack& stack, Context context);
	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	Carousel				mCarousel;
};