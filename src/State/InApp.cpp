#include <State/InApp.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>
#include <cstdlib>
#include <random>
#include <iostream>

InApp::InApp(StateStack& stack, Context context, World::Mode mode): 
    State(stack, context),
    mWorld(*context.window, *context.textures,*context.fonts)
{   
    mWorld.setMode(mode);
}

void InApp::draw()
{
	mWorld.draw();
}

bool InApp::update(sf::Time dt)
{

    mWorld.update(dt);
	return true;
}

bool InApp::handleEvent(const sf::Event& event)
{
    mWorld.handleEvent(event);

    if (mWorld.getBackRequest())
    {
        requestStackPop();
        requestStackPush(States::Menu);
    }
	return true;
}
