#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>
#include <SFML/Audio.hpp>

class App
{
public:
                            App();
    void                    run();

private:
    sf::ContextSettings     mContextSettings;
    sf::RenderWindow        mWindow;
    TextureHolder           mTextures;
    FontHolder              mFonts;
    sf::Music               mMusic;

private:
    bool                    mIsPaused;

private:
    void                    loadTextures();
    void                    loadFonts();
    void                    loadMusic();

private:
    void                    handleEvent();
    void                    update(sf::Time dt);
    void                    draw();

private:
    StateStack              mStateStack;
    void                    registerStates();

private:
    sf::View                mView;
    sf::FloatRect           mBounds;
};  
