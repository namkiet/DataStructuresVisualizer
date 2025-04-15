#include <Core/ActionQueue.hpp>
#include <Core/Variables.hpp>
#include <iostream>

void ActionQueue::empty()
{
    while (!queue.empty())
        queue.pop_back();
}

void ActionQueue::pushAction(ActionFunc action, bool withPrevious)
{
    if (ANIMATION::Speed >= 1000)
    {
        sf::Time dt;
        action(dt);
        return;
    }

    

    if (queue.empty() || !withPrevious)
        queue.emplace_back();
    queue.back().push_back(std::move(action));
}

void ActionQueue::pushInstantAction(std::function<void()> func, bool withPrevious)
{
    if (ANIMATION::Speed >= 1000)
    {
        func();
        return;
    }
    
    if (!withPrevious)   
        queue.emplace_back();
    pushAction([func](sf::Time) { func(); return true; }, true);
}

float ActionQueue::update(sf::Time dt)
{
    if (!queue.empty())
    {

        auto &currentBatch = queue.front();
        bool allFinished = true;

        for (auto currentBatchIterator = currentBatch.begin(); currentBatchIterator != currentBatch.end(); )
        {
            if ((*currentBatchIterator)(dt))
            {
                currentBatchIterator = currentBatch.erase(currentBatchIterator);
            }
            else
            {
                ++currentBatchIterator;
                allFinished = false;
                timer += dt.asSeconds();
            }
        }

        if (allFinished)
        {
            queue.pop_front();

            float t = timer;
            timer = 0;
            

            if (t != 0) return t;
            else return -2; // finish in 0s
        }
        else return -1; // not finished
    }
    mSize = 0;
    return 0; // empty
}

void ActionQueue::createNewBatch()
{
    if (ANIMATION::Speed >= 1000.f) return;
    queue.emplace_back();
}

bool ActionQueue::isEmpty() const
{
    return queue.empty();
}

int ActionQueue::size() const
{
    // return mSize;
    auto temp = queue;
    int cnt = 0;
    while (!temp.empty())
    {
        if (!temp.front().empty()) cnt++;
        temp.pop_front();
    }
    return cnt;
    // return queue.size();
}