#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>

#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>

class SettingsState : public State
{
public:
							SettingsState(StateStack& stack, Context context);
	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);

private:
	void 					initNodesAndEdge();
	CircleNode*				node1;
	CircleNode*				node2;
	Edge*					edge;
};