#include <Core/App.hpp>
#include <State/TitleState.hpp>
#include <State/MenuState.hpp>
#include <State/AVL.hpp>
#include <State/Graph.hpp>
#include <State/Heap.hpp>
#include <State/LinkedList.hpp>
#include <iostream>

App::App(): 
    mWindow(sf::VideoMode(1200, 720), "My App"),
    mStateStack(State::Context(mWindow, mTextures, mFonts)),
    // mBounds(sf::FloatRect(100, 100, 1100, 620)),
    mView(mWindow.getDefaultView())
{
    loadTextures();
    loadFonts();
    registerStates();
    mStateStack.pushState(States::Title);
}

void App::loadTextures()
{
    mTextures.load(Textures::TitleScreen, "assets/images/TitleScreen.png");
}

void App::loadFonts()
{
    mFonts.load(Fonts::Main, "assets/fonts/jetbrains.ttf");
}


void App::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<AVL>(States::AVL);
    mStateStack.registerState<Graph>(States::Graph);
    mStateStack.registerState<Heap>(States::Heap);
    mStateStack.registerState<LinkedList>(States::LinkedList);
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
    mWindow.clear(sf::Color(173, 126, 230));
    mStateStack.draw();
    mWindow.display();
}