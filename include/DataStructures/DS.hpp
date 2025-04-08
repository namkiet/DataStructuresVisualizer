#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/CircleNode.hpp>
#include <SceneNode/Edge.hpp>
#include <Core/ActionQueue.hpp>
#include <Core/Action.hpp>

#include <SceneNode/TreeNode.hpp>
#include <iostream>
#include <string>

#include <Core/Variables.hpp>

class DS: public SceneNode
{
// ==================== BASIC FUNCTIONS ====================
public:
    virtual void                    insert(int value) = 0;
    virtual void                    remove(int value) = 0;
    virtual bool                    search(int value) = 0;

    virtual void                    empty();
    void                            loadFromVector(std::vector<int> numList);

private:
    virtual void                    align() {};
    virtual void                    updateCurrent(sf::Time dt) override;
    virtual void                    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;


// ==================== DATA STORAGE ====================
protected:
    ActionQueue                     mActionQueue;
    std::vector<CircleNode::Ptr>    mNodeList;
    std::vector<Edge::Ptr>          mEdgeList;


// ==================== ACTION METHODS ====================
protected:
    void                            createNewActionGroup();

    // Node actions
    void                            addNode(CircleNode* node);
    void                            removeNode(CircleNode* node);
    void                            moveNode(CircleNode* node, sf::Vector2f targetPos, float duration);
    void                            highlightNode(CircleNode* node, sf::Color highlightColor, float duration, bool reverse = true);
    void                            swapTwoNodes(CircleNode* a, CircleNode* b);
    void                            deleteNodeEffect(CircleNode* node, float duration);
    void                            deleteNode(CircleNode* node);

    // Edge actions
    void                            addEdge(CircleNode* parent, CircleNode* child, bool hasArrow = false);
    void                            removeEdge(CircleNode* parent, CircleNode* child); 
    void                            moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);
    void                            traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration);


// ==================== UNDO / REDO ====================
protected:
    struct History
    {
        std::vector<CircleNode::Ptr> nodeList;
        std::vector<Edge::Ptr> edgeList;
        CircleNode* baseNode;

        History(std::vector<CircleNode::Ptr>&& nodeList, std::vector<Edge::Ptr>&& edgeList, CircleNode* baseNode)
        : nodeList(std::move(nodeList)), edgeList(std::move(edgeList)), baseNode(baseNode) {};
    };

    std::stack<History>             mUndoStack;
    std::stack<History>             mRedoStack;

public:
    bool                            canUndo();
    bool                            canRedo();
    void                            undo();
    void                            redo();

protected:
    void                            execute();
    virtual void                    saveState(std::stack<History> &stack) = 0;
    virtual void                    loadState(History history) = 0;

    
// ==================== INFO / PSEUDOCODE SUPPORT ====================
protected:
    std::string                     mInfo = "";
    std::vector<std::string>        mCode = {};
    int                             mStep = -1;
    int                             mLast = -1;

public:
    std::string                     getInfo();
    std::vector<std::string>        getCode();
    int                             getStep();
};
