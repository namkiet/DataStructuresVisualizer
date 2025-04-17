#include <Core/App.hpp>
#include <State/TitleState.hpp>
#include <State/MenuState.hpp>
#include <State/SettingsState.hpp>
#include <State/InApp.hpp>
#include <Core/Variables.hpp>
#include <iostream>

App::App(): 
    
    mContextSettings(0, 0, 16),
    mWindow(sf::VideoMode(SCREEN::Width, SCREEN::Height), "My App", sf::Style::Default, mContextSettings),
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
    mTextures.load(Textures::HomeIcon, "assets/images/home.png");

	mTextures.load(Textures::AppBackground1, "assets/images/desert-night.png");
    mTextures.load(Textures::AppBackground2, "assets/images/sunrise.jpg");
    mTextures.load(Textures::AppBackground3, "assets/images/forest.jpg");
	mTextures.load(Textures::MenuBackground, "assets/images/menu-bg (1).png");

    mTextures.load(Textures::AVLThumbnail, "assets/images/avl-thumbnail.png");
    mTextures.load(Textures::HeapThumbnail, "assets/images/heap-thumbnail.png");
    mTextures.load(Textures::LLThumbnail, "assets/images/ll-thumbnail.png");
    mTextures.load(Textures::GraphThumbnail, "assets/images/graph-thumbnail.png");

    mTextures.load(Textures::Prev, "assets/images/prevFrame.png");
    mTextures.load(Textures::Next, "assets/images/nextFrame.png");
    mTextures.load(Textures::First, "assets/images/beginning.png");
    mTextures.load(Textures::Last, "assets/images/end.png");
    mTextures.load(Textures::Pause, "assets/images/pause.png");
    mTextures.load(Textures::Play, "assets/images/play.png");

    mTextures.load(Textures::SettingIcon, "assets/images/settings.png");
    mTextures.load(Textures::NavigateLeft, "assets/images/navigate_left.png");
    mTextures.load(Textures::NavigateRight, "assets/images/navigate_right.png");
    mTextures.load(Textures::ID::Minus, "assets/images/minus.jpg");
    mTextures.load(Textures::ID::Plus, "assets/images/plus.jpg");
}

void App::loadFonts()
{
    mFonts.load(Fonts::Main, "assets/fonts/jetbrains.ttf");
    mFonts.load(Fonts::UI, "assets/fonts/Poppins-Regular.ttf");
}

void App::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<SettingsState>(States::Settings);
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
    mWindow.clear(SCREEN::BackgroundColor);
    mStateStack.draw();
    mWindow.display();
}