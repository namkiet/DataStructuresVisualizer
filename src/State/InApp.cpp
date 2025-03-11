#include <State/InApp.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/Utility.hpp>
#include <cstdlib>
#include <random>
#include <iostream>

InApp::InApp(StateStack& stack, Context context): 
    State(stack, context),
    mWorld(*context.window, *context.textures,*context.fonts),
    mUser()
{

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
    // if (event.type == sf::Event::KeyPressed)
    // {
    //     if (event.key.code == sf::Keyboard::Escape)
    //     {   
    //         requestStackPop();
    //         requestStackPush(States::Menu);
    //     }

    //     if (event.key.code == sf::Keyboard::A)
    //     {
    //         // int value = std::rand() % 100;
    //         int value = v[id];
    //         id++;
    //         std::cerr << value << "\n";
    //         mInApp->insert(value);
    //     }

    //     if (event.key.code == sf::Keyboard::B)
    //         mInApp->setPosition(mInApp->getPosition() - sf::Vector2f(50, 0));

    //     if (event.key.code == sf::Keyboard::C)
    //         mInApp->leftRotate();

    //     if (event.key.code == sf::Keyboard::D)
    //     {
    //         if (mInApp->search(5))
    //             std::cerr << "Found \n";
    //         else 
    //             std::cerr << "Cannot find \n";
    //     }
    // }

	return true;
}
