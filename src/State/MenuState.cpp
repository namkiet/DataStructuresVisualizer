#include <State/MenuState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "State/InApp.hpp"
#include <iostream>

MenuState::MenuState(StateStack& stack, Context context): 
	State(stack, context), 
	mOptions(), 
	mOptionIndex(0)
{
	sf::Texture& texture = context.textures->get(Textures::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::Main);

	mBackgroundSprite.setTexture(texture);
	
	sf::Text AVLOption;
	AVLOption.setFont(font);
	AVLOption.setString("AVL");
	centerOrigin(AVLOption);
	AVLOption.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, -90.f));

	sf::Text HeapOption;
	HeapOption.setFont(font);
	HeapOption.setString("Heap");
	centerOrigin(HeapOption);
	HeapOption.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, -30.f));

	sf::Text LinkedListOption;
	LinkedListOption.setFont(font);
	LinkedListOption.setString("LinkedList");
	centerOrigin(LinkedListOption);
	LinkedListOption.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, 30.f));

	sf::Text GraphOption;
	GraphOption.setFont(font);
	GraphOption.setString("Graph");
	centerOrigin(GraphOption);
	GraphOption.setPosition(context.window->getView().getSize() / 2.f + sf::Vector2f(0.f, 90.f));


	mOptions.push_back(AVLOption);
	mOptions.push_back(HeapOption);
	mOptions.push_back(LinkedListOption);
	mOptions.push_back(GraphOption);

	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
	// window.draw(mBackgroundSprite);

	for (auto text: mOptions)
		window.draw(text);
	
}

bool MenuState::update(sf::Time)
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// The demonstration menu logic
	if (event.type != sf::Event::KeyPressed)
		return false;

	if (event.key.code == sf::Keyboard::Return)
	{
		requestStackPop();
		switch (mOptionIndex)
		{
			case AVL:
				requestStackPush(States::InAppAVL);
				break;
			
			case Heap:
				requestStackPush(States::InAppHeap);
				break;


			case LinkedList:
				requestStackPush(States::InAppLinkedList);
				break;

			case Graph:
				requestStackPush(States::InAppGraph);
				break;
			
			default:
				break;
				

		}
	}

	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement and wrap-around
		if (mOptionIndex > 0)
			mOptionIndex--;
		else
			mOptionIndex = mOptions.size() - 1;

		updateOptionText();
	}

	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment and wrap-around
		if (mOptionIndex < mOptions.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		updateOptionText();
	}

	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// White all texts
	for (auto &text: mOptions)
		text.setColor(sf::Color::White);

	// Red the selected text
	mOptions[mOptionIndex].setColor(sf::Color::Blue);
}