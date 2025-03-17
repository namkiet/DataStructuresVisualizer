#include <SFML/System/Time.hpp>
#include <queue>
#include <functional>

class ActionQueue {
private:
    std::deque<std::vector<std::function<bool(sf::Time)>>> queue;

public:
    void pushAction(std::function<bool(sf::Time)> action)
    {
        if (queue.empty())
            queue.emplace_back();

        queue.back().push_back(std::move(action));
    }

    void update(sf::Time dt) {
        if (!queue.empty()) {
            auto &currentBatch = queue.front();
            bool allFinished = true;

            for (auto it = currentBatch.begin(); it != currentBatch.end(); ) {
                if ((*it)(dt)) {
                    it = currentBatch.erase(it);
                } else {
                    ++it;
                    allFinished = false;
                }
            }

            if (allFinished) {
                queue.pop_front();
            }
        }
    }

    void createNewBatch()
    {
        queue.emplace_back();
    }

    bool empty() const { return queue.empty(); }
};
