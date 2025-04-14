#include <Core/ActionQueue.hpp>
#include <Core/Variables.hpp>

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
        timer += dt.asSeconds();

        auto &currentBatch = queue.front();
        bool allFinished = true;

        for (auto currentBatchIterator = currentBatch.begin(); currentBatchIterator != currentBatch.end(); )
        {
            if ((*currentBatchIterator)(dt))
                currentBatchIterator = currentBatch.erase(currentBatchIterator);
            else
            {
                ++currentBatchIterator;
                allFinished = false;
            }
        }

        if (allFinished)
        {
            queue.pop_front();
            // if (timer != dt.asSeconds())
            // {
                float t = timer;
                timer = 0;
                return 1;
            // }
            // return true;
        }
    }
    return 0;
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
    return queue.size();
}