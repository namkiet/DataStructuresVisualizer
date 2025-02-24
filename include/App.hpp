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

    StateStack          mStateStack;
    void                registerStates();

private:
    void                handleEvent();
    void                update(sf::Time dt);
    void                draw();

private:
    void                loadTextures();
    void                loadFonts();

private:
    sf::View            mView;
    sf::FloatRect       mBounds;
};  
