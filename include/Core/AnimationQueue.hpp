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
    std::queue<std::vector<std::unique_ptr<Animation>>> queue;

public:
    void addAnimation(std::unique_ptr<Animation> anim)
    {
        std::vector<std::unique_ptr<Animation>> newAnimation;
        newAnimation.push_back(std::move(anim));
        // queue.push(std::move(anim));
        queue.push(std::move(newAnimation));
    }

    void addAnimationGroup(std::vector<std::unique_ptr<Animation>> &animGroup)
    {
        queue.push(std::move(animGroup));
    }

    void update(sf::Time dt) 
    {
        if (!queue.empty()) {
            auto& curAnimationGroup = queue.front();
            bool isFinished = true;
            for (int i = 0; i < curAnimationGroup.size(); i++)
            {
                if (!curAnimationGroup[i]->update(dt)) isFinished = false;
            }
            
            if (isFinished) queue.pop();
            // if (queue.front()->update(dt)) {
            //     queue.pop(); // Move to the next animation when finished
            // }
        }
    }

    bool isEmpty() const 
    {
        return queue.empty();
    }
};