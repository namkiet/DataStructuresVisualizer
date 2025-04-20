#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>

#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include<vector>
class SettingsState : public State
{
public:
							SettingsState(StateStack& stack, Context context);
	virtual void			draw();
	virtual bool			update(sf::Time dt);
	virtual bool			handleEvent(const sf::Event& event);
	void 					updateStatDisplay();

private:
	Context										context;
	void 										initNodesAndEdge();
	std::vector<CircleNode*>					nodes;
	std::vector<Edge*>							edges;
	GUI::Button::Ptr        					mHomeButton;

	sf::Sprite									mBackgroundSprite;
	sf::Text									mBackgroundTextOption;

	GUI::Button::Ptr                            BgNavigateLeft;
	GUI::Button::Ptr                            BgNavigateRight;
	GUI::Button::Ptr							DecreaseNodeSize;
	GUI::Button::Ptr							IncreaseNodeSize;
	GUI::Button::Ptr							DecreaseEdgeThickness;
	GUI::Button::Ptr							IncreaseEdgeThickness;
	GUI::Button::Ptr							mSoundButton;


	sf::Text									NodeSize;
	sf::Text									EdgeThickness;

	sf::Text									mBackgroundText;
	sf::Text									mNodeSizeText;
	sf::Text 									mEdgeThicknessText;
	sf::Text									mSoundText;

	

};