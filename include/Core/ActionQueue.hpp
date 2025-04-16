#include <SFML/System/Time.hpp>
#include <queue>
#include <stack>
#include <functional>
#include <iostream>
#include <Core/Action.hpp>

class ActionQueue 
{
public:
    // using ActionFunc = std::function<bool(sf::Time)>;

private:
    std::deque<std::vector<Action::ActionFunc>> queue;
    float                                       timer;
    int                                         mSize = 0;

public:
    void                                        empty();
    float                                       getFrontTime();
    float                                       getTotalTime();
    void                                        pushAction(Action::ActionFunc action, bool withPrevious = true);
    void                                        pushInstantAction(std::function<void()> func, bool withPrevious = false);
    float                                       update(sf::Time dt);
    void                                        createNewBatch();
    bool                                        isEmpty() const;
    int                                         size() const;
};
