#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include <Core/Animation.hpp>
#include <Core/ActionQueue.hpp>
#include <Core/Action.hpp>

#include <SceneNode/TreeNode.hpp>
#include <iostream>

class DS: public SceneNode
{
public:
    virtual void                    insert(int value) = 0;
    virtual void                    remove(int value) = 0;
    virtual bool                    search(int value) = 0;
    virtual void                    empty();
    void                            loadFromVector(std::vector<int> numList);

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
    std::vector<CircleNode*>        mNodeList;
    std::vector<Edge*>              mEdgeList;

protected:
    void                            createNewActionGroup();

    void                            addNode(CircleNode* node);
    void                            removeNode(CircleNode* node);
    void                            moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect);
    void                            highlightNode(CircleNode* node, sf::Color highlightColor, float duration);
    void                            swapTwoNodes(CircleNode* a, CircleNode* b);

    void                            addEdge(CircleNode* parent, CircleNode* child);
    Edge*                           findEdge(CircleNode* parent, CircleNode* child);
    void                            removeEdge(CircleNode* parent, CircleNode* child); 
    void                            moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);
    void                            traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration);

public:
    void undo();

protected:
    struct History
    {
        std::vector<CircleNode::Ptr> nodeList;
        std::vector<Edge::Ptr> edgeList;
        CircleNode* baseNode;

        History(std::vector<CircleNode::Ptr>&& nodeList, std::vector<Edge::Ptr>&& edgeList, CircleNode* baseNode)
        : nodeList(std::move(nodeList)), edgeList(std::move(edgeList)), baseNode(baseNode) {};
    };

    std::stack<History>             mHistory;
    virtual void                    saveState() = 0;
    virtual void                    loadState() = 0;
};

