#include <Core/ActionQueue.hpp>

void ActionQueue::pushAction(ActionFunc action)
{
    if (queue.empty())
        queue.emplace_back();

    queue.back().push_back(std::move(action));
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

bool ActionQueue::empty() const
{
    return queue.empty();
}