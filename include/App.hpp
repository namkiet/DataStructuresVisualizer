#pragma once
#include <SFML/Graphics.hpp>
#include <StateStack.hpp>

class App
{
public:
                        App();
    void                run();

private:
    sf::RenderWindow    mWindow;
    TextureHolder       mTextures;
    FontHolder          mFonts;

private:
    bool                mIsPaused;

private:
    void                loadTextures();
    void                loadFonts();

private:
    void                handleEvent();
    void                update(sf::Time dt);
    void                draw();

private:
    StateStack          mStateStack;
    void                registerStates();

private:
    sf::View            mView;
    sf::FloatRect       mBounds;
};  
