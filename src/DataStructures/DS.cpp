#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>
#include <iostream>

void DS::empty()
{
    mEdgeList.clear();
    mNodeList.clear();
    mActionQueue.empty();
}

bool DS::isRunning()
{
    return !mActionQueue.isEmpty();
}

void DS::loadState(float progress)
{
    if (mUndoStack.empty()) return;

    curId = int(progress * (mUndoStack.size() - 1));
    loadState(mUndoStack[curId]);
}

void DS::loadState(History history)
{
    empty();
    mNodeList = std::move(history.nodeList);
    mEdgeList = std::move(history.edgeList);
    mInfo = std::move(history.info);
    mStep = std::move(history.step);
}

bool DS::canUndo()
{
    return curId >= 1;
}

bool DS::canRedo()
{
    return curId < mUndoStack.size() - 1;
}

void DS::execute()
{
    saveState(mUndoStack);
    std::cerr << "Undo size: " << mUndoStack.size() << "\n";
}

void DS::undo()
{
    // if (!canUndo()) return;
    // loadState(mUndoStack[--curId]);
}

void DS::redo()
{
    // if (!canRedo()) return;
    // loadState(mUndoStack[++curId]);
}

float DS::getProgress()
{
    if (currentHistorySize <= 1) return 1.f;

    float progress = (curId * 1.f) / (currentHistorySize - 1);

    std::cerr << "Cur id: " << curId << "_ " << currentHistorySize << " " << " - Progress: " << progress << "\n";

    return progress;
}

void DS::updateCurrent(sf::Time dt)   
{
    std::cerr << mNodeList.size() << "\n";
    if (mActionQueue.isEmpty())
    {
        mStep = mLastStep;
        if (mLastInfo != "#") mInfo = mLastInfo;

        currentHistorySize = 0;
        curId = 0;

    }
    else 
    {
        // // timer += dt.asSeconds();
        // if (currentHistorySize == 0)
        //     currentHistorySize = mActionQueue.size();

        // execute();
        float t = mActionQueue.update(dt);
        if (t)
        {
            execute();
        }
        // //     // timer = t;

        // //     std::cerr << "time: " << t << "\n";

        // //     execute();
        // //     curId = mUndoStack.size() - 1;

        // //     std::cerr << "???: " << curId << "\n";
        // // }

        // if (!t) 
        // {
        //     mUndoStack.pop_back();
        // }
    }

    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);

    for (auto &node: mNodeList)
        if (node) node->update(dt);

}

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    states.transform *= getTransform();  
    
    for (auto &edge: mEdgeList)
        if (edge) edge->draw(target, states);

    for (auto &node: mNodeList)
        if (node) node->draw(target, states);
}

void DS::addNode(CircleNode* node)
{
    mNodeList.push_back(CircleNode::Ptr(node));
}

void DS::removeNode(CircleNode* node)
{
    mActionQueue.pushInstantAction([=]()
    {
        mEdgeList.erase(
            std::remove_if(mEdgeList.begin(), mEdgeList.end(),
                [node](const Edge::Ptr& edge) {
                    return edge->mFrom == node;
                }),
            mEdgeList.end()
        );

        mNodeList.erase(
            std::remove_if(mNodeList.begin(), mNodeList.end(),
            [node](const CircleNode::Ptr& ptr) { return ptr.get() == node; }),
            mNodeList.end()
        );
    });
}

void DS::addEdge(CircleNode* parent, CircleNode* child, bool hasArrow) 
{
    mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, hasArrow, VIZ::EDGE::Thickness));
}

void DS::removeEdge(CircleNode* parent, CircleNode* child) {
    mActionQueue.pushInstantAction([=]() 
    {
        mEdgeList.erase(
            std::remove_if(mEdgeList.begin(), mEdgeList.end(),
            [parent, child](const Edge::Ptr& edge) {
                return edge->mFrom == parent && edge->mTo == child;
            }),
            mEdgeList.end()
        );
    });
}

void DS::createNewActionGroup()
{
    mActionQueue.createNewBatch();
}

void DS::highlightNode(CircleNode* node, sf::Color highlightColor, float duration, bool reverse)
{
    mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration, reverse));
}

void DS::deleteNodeEffect(CircleNode* node, float duration) // remove node from mNodeList and create dissapear effect
{
    std::cout<<"start Delete node effect\n";
    mActionQueue.pushAction(Action::DeleteNode(node, duration));
    std::cout<<"end Delete node effect\n";
}

void DS::deleteNode(CircleNode* node){
    std::cout<<"start Delete node\n";
    mActionQueue.pushAction([this, node](sf::Time dt) mutable -> bool
    {
        mNodeList.erase(
        std::remove_if(mNodeList.begin(), mNodeList.end(),
        [node](const CircleNode::Ptr& n) {
            return n.get() == node;
        }),
        mNodeList.end()
    );
    std::cout<<"end Node removed\n";
        return true;
    });

}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration)
{
    if (!node) return;
    sf::Vector2f prevPos = node->getPosition();
    mActionQueue.pushAction(Action::MoveNode(node, targetPos, duration));
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
{
    mActionQueue.pushAction(Action::MoveEdge(mEdgeList, parent, child, targetTail, duration)); 
}

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
}

void DS::swapTwoNodes(CircleNode* a, CircleNode* b, float duration)
{
    if (!a || !b) return;

    int aVal = a->mValue;
    int bVal = b->mValue;

    a->mValue = bVal;
    b->mValue = aVal;

    int aOpa = a->getOpacity();
    int bOpa = b->getOpacity();

    // Create 2 fake nodes
    TreeNode* fakeA = new TreeNode(aVal, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    TreeNode* fakeB = new TreeNode(bVal, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    fakeA->setOpacity(0);
    fakeB->setOpacity(0);
    fakeA->setPosition(a->getPosition());
    fakeB->setPosition(b->getPosition());
    addNode(fakeA);
    addNode(fakeB);

    // Make the real nodes transparent and make the two fake nodes appear
    mActionQueue.pushInstantAction([=]() {
        a->setOpacity(0);
        b->setOpacity(0);
        fakeA->setOpacity(1);
        fakeB->setOpacity(1);
    });

    // Swap the 2 fake nodes
    createNewActionGroup();
    moveNode(fakeA, b->getPosition(), duration);
    moveNode(fakeB, a->getPosition(), duration);

    // Remove the 2 fake nodes
    createNewActionGroup();
    removeNode(fakeA);
    removeNode(fakeB);

    // Make the real nodes appear again
    mActionQueue.pushInstantAction([=]() {
        a->setOpacity(aOpa);
        b->setOpacity(bOpa);
        a->setValue(bVal);
        b->setValue(aVal);
    });
}

void DS::loadFromVector(std::vector<int> numList)
{
    empty();
    auto curSpeed = ANIMATION::Speed;
    ANIMATION::Speed = 1000;
    for (int x: numList) insert(x);
    mActionQueue.pushInstantAction([=](){ ANIMATION::Speed = curSpeed; align(); });
}    

std::string DS::getInfo()
{
    return mInfo;
}

std::vector<std::string> DS::getCode()
{
    return mCode;
}

int DS::getStep()
{
    return mStep;
}