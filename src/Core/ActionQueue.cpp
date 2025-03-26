#include <Core/ActionQueue.hpp>

void ActionQueue::pushAction(ActionFunc action)
{
    std::cout<<"start Push action\n";
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

        for (auto batchIterator = currentBatch.begin(); batchIterator != currentBatch.end(); )
        {
            if ((*batchIterator)(dt))
                batchIterator = currentBatch.erase(batchIterator);
            else
            {
                ++batchIterator;
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