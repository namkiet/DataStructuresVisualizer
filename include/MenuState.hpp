#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <StateStack.hpp>


class MenuState : public State
{
public:
							MenuState(StateStack& stack, Context context);
	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	void					updateOptionText();

private:
	enum OptionNames
	{
		AVL,
		Heap,
		LinkedList,
		Graph
	};


private:
	sf::Sprite				mBackgroundSprite;

private:
	std::vector<sf::Text>	mOptions;
	std::size_t				mOptionIndex;
};