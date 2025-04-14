#include <SFML/System/Time.hpp>
#include <queue>
#include <stack>
#include <functional>
#include <iostream>
class ActionQueue 
{
public:
    using ActionFunc = std::function<bool(sf::Time)>;

private:
    std::deque<std::vector<ActionFunc>> queue;
    float timer;

public:
    void empty();
    void pushAction(ActionFunc action, bool withPrevious = true);
    void pushInstantAction(std::function<void()> func, bool withPrevious = false);
    float update(sf::Time dt);
    void createNewBatch();
    bool isEmpty() const;
    int size() const;
};
