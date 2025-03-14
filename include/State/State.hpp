#pragma once

#include <SFML/Graphics.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <memory>

namespace States {
    enum ID {
        None,
        InApp,
        // MainMenu, 
        InAppAVL,
        InAppGraph,
        InAppHeap,
        InAppLinkedList,
        Title,
        Menu
        // Settings,
        // StateCount
    };
};

// Forward declaration
class StateStack;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;
    struct Context
    {
                                    Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);

        sf::RenderWindow*	        window;
        TextureHolder*		        textures;
        FontHolder*		            fonts;
    };


public:
                                    State(StateStack& stack, Context context);
    virtual                         ~State();

public:
    virtual bool                    handleEvent(const sf::Event& event) = 0;
    virtual bool                    update(sf::Time dt) = 0;
    virtual void                    draw() = 0;

protected:
    void                            requestStackPush(States::ID stateID);
    void                            requestStackPop();
    void                            requestStateClear();

    Context                         getContext() const;

private:
    StateStack*                     mStack;
    Context                         mContext;
};