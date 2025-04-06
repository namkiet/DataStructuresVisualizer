#include <Core/ActionQueue.hpp>

void ActionQueue::empty()
{
    while (!queue.empty())
        queue.pop_back();
}

void ActionQueue::pushAction(ActionFunc action, bool withPrevious)
{
    if (queue.empty() || !withPrevious)
        queue.emplace_back();
    queue.back().push_back(std::move(action));
}

void ActionQueue::pushInstantAction(std::function<void()> func, bool withPrevious)
{
    if (!withPrevious)   
        queue.emplace_back();
    pushAction([func](sf::Time) { func(); return true; }, true);
}

void ActionQueue::update(sf::Time dt)
{
    if (!queue.empty())
    {
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
            queue.pop_front();
    }
}

void ActionQueue::createNewBatch()
{
    queue.emplace_back();
}

bool ActionQueue::isEmpty() const
{
    return queue.empty();
}