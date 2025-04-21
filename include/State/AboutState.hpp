#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>

#include<vector>
class AboutState : public State
{
public:
							                    AboutState(StateStack& stack, Context context);
	virtual void			                    draw();
    virtual bool			                    update(sf::Time dt);
	virtual bool			                    handleEvent(const sf::Event& event);
private:
	Context										context;
	GUI::Button::Ptr        					mHomeButton;

    sf::RectangleShape                          mDescriptionBox;
	sf::Sprite									mBackground;
    sf::Text                                    mProjectDescription;
    std::vector<sf::Text>                       mTeamTexts;
    sf::CircleShape                             mBullet1;
    sf::CircleShape                             mBullet2;
    sf::CircleShape                             mBullet3;
    sf::Text                                    mAboutTitle;

};