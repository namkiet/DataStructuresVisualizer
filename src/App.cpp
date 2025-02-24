#include <App.hpp>
#include <TitleState.hpp>
#include <MenuState.hpp>
#include <AVL.hpp>
#include <Graph.hpp>
#include <Heap.hpp>
#include <LinkedList.hpp>
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
        update(dt);
        if (mStateStack.isEmpty()) mWindow.close();
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