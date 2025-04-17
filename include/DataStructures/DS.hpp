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
public:
    bool isStepByStep = false;

    bool stop;
    void sBs();

    enum RunType { UNDO, REDO, NORMAL };
    RunType playback;

// ==================== BASIC FUNCTIONS ====================
public:
                                    DS();
    bool                            isRunning();
    virtual void                    insert(int value) = 0;
    virtual void                    remove(int value) = 0;
    virtual bool                    search(int value) = 0;
    virtual void                    updateValue(int value, int newValue) {};

    virtual void                    empty();
    virtual void                    loadFromVector(std::vector<int> numList);

protected:
    virtual void                    align() {};
    virtual void                    updateCurrent(sf::Time dt) override;
    virtual void                    drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void                            changeNodePosition(int id, sf::Vector2f Pos);
    sf::Vector2f                    getWorldPosition(int id);

// ==================== DATA STORAGE ====================
protected:
    std::vector<CircleNode::Ptr>    mNodeList;
    std::vector<Edge::Ptr>          mEdgeList;

// ==================== ACTION METHODS ====================
protected:
    ActionQueue                     mActionQueue;
    void                            createNewActionGroup();

    // Node actions
    void                            addNode(CircleNode* node);
    void                            removeNode(CircleNode* node);

    void                            moveNode(CircleNode* node, sf::Vector2f targetPos, float duration);
    void                            highlightNode(CircleNode* node, sf::Color highlightColor, float duration, bool reverse = true);
    void                            changeNodeColor(CircleNode* node, sf::Color highlightColor, float duration);
    void                            swapTwoNodes(CircleNode* a, CircleNode* b, float duration);
    void                            deleteNodeEffect(CircleNode* node, float duration);
    void                            deleteNode(CircleNode* node);

    // Edge actions
    void                            addEdge(CircleNode* parent, CircleNode* child, bool hasArrow = false);
    void                            addEdge(CircleNode* parent, CircleNode* child, int weight, bool hasArrow);

    void                            removeEdge(CircleNode* parent, CircleNode* child); 
    void                            moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration);
    void                            traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration, bool reverse = true);
    void                            changeEdgeColor(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration);

// ==================== STEP-BY-STEP ====================
public:
    bool                            canUndo();
    bool                            canRedo();
    void                            undo();
    void                            redo();
    void                            loadStep(float progress);
    float                           getProgress();

private:
    void                            saveStep();
    bool                            isReverse = false;
    std::vector<sf::Texture>        mH;
    int                             cS = -1;
    float                           timer;
    float                           elapsedTimer = 0;
    float                           totalTimer = 0;
    int                             targetFrame = -1;

    std::vector<int>                keyFrames;
    int                             keyID = -1;

    int mActionCount;
    int mCurrentAction = 0;

protected:
    void                            resetHistory();
    
// ==================== INFO / PSEUDOCODE SUPPORT ====================
protected:
    std::string                     mInfo = "";
    std::vector<std::string>        mCode = {};
    int                             mStep = -1;
    int                             mLastStep = -1;
    std::string                     mLastInfo = "";

public:
    std::string                     getInfo();
    std::vector<std::string>        getCode();
    int                             getStep();
};
