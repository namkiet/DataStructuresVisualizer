#pragma once
#include <SFML/Graphics.hpp>
#include <queue>
#include <memory>
#include <functional>

class Animation {
protected:
    float                       duration;
    float                       elapsed;
    bool                        finished;
    bool                        isInit;

public:
    virtual                     ~Animation() {}
    virtual bool                update(sf::Time dt) = 0;
};

class AnimationQueue {
private:
    std::queue<std::unique_ptr<Animation>> queue;

public:
    void addAnimation(std::unique_ptr<Animation> anim)
    {
        queue.push(std::move(anim));
    }

    void update(sf::Time dt) 
    {
        if (!queue.empty()) {
            if (queue.front()->update(dt)) {
                queue.pop(); // Move to the next animation when finished
            }
        }
    }

    bool isEmpty() const 
    {
        return queue.empty();
    }
};