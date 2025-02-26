#pragma once

#include <SFML/Graphics.hpp>
#include <State/State.hpp>
#include <functional>

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };

public:
    explicit                        StateStack(State::Context context);
    template <typename T>
    void                            registerState(States::ID stateID);

public:
    void                            pushState(States::ID stateID);
    void                            popState();
    void                            clearStates();
    bool                            isEmpty() const;

private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();

private:
    struct PendingChange
    {
        explicit			        PendingChange(Action action, States::ID stateID = States::None);

        Action                      action;
        States::ID                  stateID;
    };

private:
    std::vector<State::Ptr>         mStack;
    std::vector<PendingChange>      mPendingList;
    State::Context                  mContext;
    std::map<States::ID, std::function<State::Ptr()>> mFactories;

public:
    void                            update(sf::Time dt);
    void                            draw();
    void                            handleEvent(const sf::Event& event);
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
	mFactories[stateID] = [this] ()
	{
		return State::Ptr(new T(*this, mContext));
	};
}