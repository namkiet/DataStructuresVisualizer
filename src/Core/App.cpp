#include <Core/App.hpp>
#include <State/TitleState.hpp>
#include <State/MenuState.hpp>
#include <iostream>
#include <State/InApp.hpp>
#include <Core/Variables.hpp>

App::App(): 
    
    mContextSettings(0, 0, 16),
    mWindow(sf::VideoMode(WINDOW::ScreenWidth, WINDOW::ScreenHeight), "My App", sf::Style::Default, mContextSettings),
    mStateStack(State::Context(mWindow, mTextures, mFonts)),
    mView(mWindow.getDefaultView()),
    mIsPaused(false)
{
    loadTextures();
    loadFonts();
    registerStates();
    mStateStack.pushState(States::Title);
}

void App::loadTextures()
{
    mTextures.load(Textures::Button, "assets/images/button.jpg");
    mTextures.load(Textures::HomeIcon, "assets/images/home.png");
}

void App::loadFonts()
{
    mFonts.load(Fonts::Main, "assets/fonts/jetbrains.ttf");
}


void App::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<InApp>(States::InAppAVL);
    mStateStack.registerState<InApp>(States::InAppHeap);
    mStateStack.registerState<InApp>(States::InAppGraph);
    mStateStack.registerState<InApp>(States::InAppLinkedList);

}

void App::run()
{
    sf::Clock clock;
    while (mWindow.isOpen())
    {
     
        sf::Time dt = clock.restart();
        handleEvent();
        
        if (!mIsPaused)
            update(dt);
        if (mStateStack.isEmpty()) 
            mWindow.close();
        draw();
    }
}

void App::handleEvent()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
        
        if (event.type == sf::Event::LostFocus)
            mIsPaused = true;
        
        if (event.type == sf::Event::GainedFocus)
            mIsPaused = false;
        
        if (event.type == sf::Event::KeyPressed)
            if (event.key.code == sf::Keyboard::P)
                mIsPaused  = !mIsPaused;
        
    }
}

void App::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void App::draw()
{
    mWindow.setView(mView);
    mWindow.clear(sf::Color(173, 216, 230));
    mStateStack.draw();
    mWindow.display();
}