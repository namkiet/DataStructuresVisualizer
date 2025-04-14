#include <State/SettingsState.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <iostream>

SettingsState::SettingsState(StateStack& stack, Context context): 
	State(stack, context)
{
	initNodesAndEdge();
}

void SettingsState::initNodesAndEdge()
{
    node1 = new CircleNode(1, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    node2 = new CircleNode(2, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
	node1->setPosition(sf::Vector2f(100, 100));
	node2->setPosition(sf::Vector2f(200, 200));
	edge = new Edge(VIZ::EDGE::Color, node1, node2, false, VIZ::EDGE::Thickness);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.setView(window.getDefaultView());
 
	window.draw(*node1);
	window.draw(*node2);
	window.draw(*edge);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::A)
		{
            requestStackPop();
			requestStackPush(States::Menu);
		}

        if (event.key.code == sf::Keyboard::B)
        {
			if (VIZ::NODE::Radius < 36.f)
			{
				float newSize = VIZ::NODE::Radius + 2.f;
				VIZ::NODE::Thickness = newSize * (VIZ::NODE::Thickness / VIZ::NODE::Radius);
				VIZ::NODE::Radius = newSize;
			}
        }

		if (event.key.code == sf::Keyboard::C)
		{
			if (VIZ::EDGE::Thickness < 7.f)
			{
				VIZ::EDGE::Thickness += 1.f;
			}
		}
	
		delete node1;
		delete node2;
		delete edge;
		initNodesAndEdge();
	}
	return true;
}