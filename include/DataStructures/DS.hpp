#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/Edge.hpp>
#include <Core/Animation.hpp>

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
    float                           mMaxWidth = 1200;
    float                           mVerticalSpacing = 100;

protected:
    AnimationQueue                  mAnimationQueue;
    std::vector<CircleNode::Ptr>    mNodeList;
    std::vector<Edge::Ptr>          mEdgeList;

protected:
    void                            addNode(CircleNode* node);
    void                            removeNode(CircleNode* node);

    void                            addEdge(CircleNode* parent, CircleNode* child);
    Edge*                           findEdge(CircleNode* parent, CircleNode* child);
    void                            removeEdge(CircleNode* parent, CircleNode* child); 
};