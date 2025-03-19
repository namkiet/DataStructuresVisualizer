#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include <Core/Animation.hpp>
#include <Core/ActionQueue.hpp>
#include <Core/Action.hpp>

class DS: public SceneNode
{
public:
    virtual void                    insert(int value) = 0;
    virtual void                    remove(int value) = 0;
    virtual bool                    search(int value) = 0;

public:
    virtual void                    updateCurrent(sf::Time dt);
    virtual void                    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    std::function<void(int)>        updateStepCallback; 

protected:
    float                           mAnimationSpeed = 0.5f;
    float                           mMaxWidth = 1200;
    float                           mVerticalSpacing = 100;

protected:
    AnimationQueue                  mAnimationQueue;
    ActionQueue                     mActionQueue;
    std::vector<CircleNode::Ptr>    mNodeList;
    std::vector<Edge::Ptr>          mEdgeList;

protected:
    void                            createNewActionGroup();

    void                            addNode(CircleNode* node);
    void                            removeNode(CircleNode* node);
    void                            moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect);
    void                            highlightNode(CircleNode* node, sf::Color highlightColor, float duration);

    void                            addEdge(CircleNode* parent, CircleNode* child);
    Edge*                           findEdge(CircleNode* parent, CircleNode* child);
    void                            removeEdge(CircleNode* parent, CircleNode* child); 
    void                            moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);
    void                            traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration);

public:
    void undo();
};