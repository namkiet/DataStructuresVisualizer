#include <Core/App.hpp>
#include <State/TitleState.hpp>
#include <State/MenuState.hpp>
#include <State/SettingsState.hpp>
#include <State/InApp.hpp>
#include <Core/Variables.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <State/AboutState.hpp>

App::App(): 
    mContextSettings(0, 0, 16),
    mWindow(sf::VideoMode(SCREEN::Width, SCREEN::Height), "My App", sf::Style::Titlebar | sf::Style::Close, mContextSettings),
    mStateStack(State::Context(mWindow, mTextures, mFonts)),
    mView(mWindow.getDefaultView()),
    mIsPaused(false)
{
    loadTextures();
    loadFonts();
    loadMusic();
    registerStates();
    mStateStack.pushState(States::Menu);
}

void App::loadTextures()
{
    mTextures.load(Textures::Title, "assets/images/title.png");
    mTextures.load(Textures::HomeIcon, "assets/images/home.png");

	mTextures.load(Textures::AppBackground1, "assets/images/desert-night.png");
    mTextures.load(Textures::AppBackground2, "assets/images/sunrise.jpg");
    mTextures.load(Textures::AppBackground3, "assets/images/forest.jpg");
	mTextures.load(Textures::MenuBackground, "assets/images/menu-bg (1).png");

    mTextures.load(Textures::DesertNightAVLThumbnail, "assets/images/desert-night/avl-thumbnail.png");
    mTextures.load(Textures::DesertNightHeapThumbnail, "assets/images/desert-night/heap-thumbnail.png");
    mTextures.load(Textures::DesertNightLLThumbnail, "assets/images/desert-night/ll-thumbnail.png");
    mTextures.load(Textures::DesertNightGraphThumbnail, "assets/images/desert-night/graph-thumbnail.png");

    mTextures.load(Textures::sunriseAVLThumbnail, "assets/images/sunrise/avl-thumbnail.png");
    mTextures.load(Textures::sunriseHeapThumbnail, "assets/images/sunrise/heap-thumbnail.png");
    mTextures.load(Textures::sunriseLLThumbnail, "assets/images/sunrise/ll-thumbnail.png");
    mTextures.load(Textures::sunriseGraphThumbnail, "assets/images/sunrise/graph-thumbnail.png");

    mTextures.load(Textures::forestAVLThumbnail, "assets/images/forest/avl-thumbnail.png");
    mTextures.load(Textures::forestHeapThumbnail, "assets/images/forest/heap-thumbnail.png");
    mTextures.load(Textures::forestLLThumbnail, "assets/images/forest/ll-thumbnail.png");
    mTextures.load(Textures::forestGraphThumbnail, "assets/images/forest/graph-thumbnail.png");

    mTextures.load(Textures::Prev, "assets/images/prevFrame.png");
    mTextures.load(Textures::Next, "assets/images/nextFrame.png");
    mTextures.load(Textures::First, "assets/images/beginning.png");
    mTextures.load(Textures::Last, "assets/images/end.png");
    mTextures.load(Textures::Pause, "assets/images/pause.png");
    mTextures.load(Textures::Play, "assets/images/play.png");

    mTextures.load(Textures::SettingIcon, "assets/images/settings.png");
    mTextures.load(Textures::NavigateLeft, "assets/images/navigate_left.png");
    mTextures.load(Textures::NavigateRight, "assets/images/navigate_right.png");
    mTextures.load(Textures::Minus, "assets/images/minus.png");
    mTextures.load(Textures::Plus, "assets/images/plus.png");

    mTextures.load(Textures::SelectedRadioButton, "assets/images/selected-radio-button.png");
    mTextures.load(Textures::UnselectedRadioButton, "assets/images/unselected-radio-button.png");
    mTextures.load(Textures::VolumeOn, "assets/images/volumeOn.png");
    mTextures.load(Textures::VolumeOff, "assets/images/volumeOff.png");
    
}

void App::loadFonts()
{
    mFonts.load(Fonts::Main, "assets/fonts/jetbrains.ttf");
    mFonts.load(Fonts::UI, "assets/fonts/Poppins-Regular.ttf");
}

void App::loadMusic(){
    if (!mMusic.openFromFile("assets/Sound/soundtrack.mp3")) {
        std::cerr << "[ERROR] Failed to load: assets/Sound/soundtrack.mp3" << std::endl;
        assert(false); // crash luôn nếu muốn
    }
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
    mStateStack.registerState<AboutState>(States::About);
}

void App::run()
{
    sf::Clock clock;
    
    mMusic.setLoop(true);  
    mMusic.setVolume(50);
    mMusic.play();

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
    mMusic.setVolume(Sound::volume);
}

void App::draw()
{
    mWindow.setView(mView);
    mWindow.clear(SCREEN::BackgroundColor);
    mStateStack.draw();
    mWindow.display();
}