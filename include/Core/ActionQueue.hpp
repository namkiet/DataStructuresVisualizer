#include <SFML/System/Time.hpp>
#include <queue>
#include <stack>
#include <functional>
#include <iostream>
class ActionQueue 
{
public:
    using ActionFunc = std::function<bool(sf::Time)>;

    void empty();
private:
    std::deque<std::vector<ActionFunc>> queue;

public:
    void pushAction(ActionFunc action);
    void pushInstantAction(std::function<void()> func);
    void update(sf::Time dt);
    void createNewBatch();
    bool isEmpty() const;
};
