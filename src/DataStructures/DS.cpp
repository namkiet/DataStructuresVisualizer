#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>
#include <iostream>

DS::DS()
{
    saveStep();
}

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

void DS::saveStep() {
    if (ANIMATION::Speed >= 1000) return;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; 

    sf::RenderTexture rt;
    rt.create(VIZ::DS::Size.x, VIZ::DS::Size.y, settings);
    rt.clear(sf::Color::Transparent);

    for (auto &edge: mEdgeList)
        if (edge) rt.draw(*edge);

    for (auto &node: mNodeList)
        if (node) rt.draw(*node);
    rt.display();

    // rt.setSmooth(true);
    mH.push_back(rt.getTexture());
}

void DS::loadStep(float progress)
{
    if (mH.empty()) return;
    cS = int(progress * (mH.size() - 1));
}

bool DS::canUndo()
{
    return cS > 0;
}

bool DS::canRedo()
{
    return cS + 1 < mH.size();
}

void DS::undo()
{
    // isReverse = true;
    if (canUndo()) cS--;
}

void DS::redo()
{
    if (canRedo()) cS++;
}

float DS::getProgress()
{
    if (mH.size() <= 1) return 1.f;
    float progress = (cS * 1.f) / (mH.size() - 1);
    return progress;
}

void DS::resetHistory()
{
    auto c = mH.back();
    mH.clear();
    mH.push_back(c);
    cS = 0;
}

void DS::updateCurrent(sf::Time dt)   
{
    if (isReverse)
    {
        timer += dt.asSeconds();
        if (timer >= 20 * dt.asSeconds())
        {
            if (cS > 0) cS--;
            else isReverse = false;

            timer = 0;
        }
        return;
    }

    if (mActionQueue.isEmpty())
    {
        mStep = mLastStep;
        if (mLastInfo != "#") mInfo = mLastInfo;
    }
    else 
    {
        timer += dt.asSeconds();
        float t = mActionQueue.update(dt);

        // if (timer >= 0.1)
        // {
        saveStep();
        cS++;
        // timer = 0;
        // }
        if (!t && timer < 0.1f)
        {
            mH.pop_back();
            cS--;
        }

        if (timer >= 0.1f) timer = 0;
    }

    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);

    for (auto &node: mNodeList)
        if (node) node->update(dt);
}

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    states.transform *= getTransform();  

    if (cS != mH.size() - 1) // not the last step
    {
        if (!mH.empty())
        {
            sf::Sprite sprite(mH[cS]);
            target.draw(sprite, states);
        }
    }
    else // last step
    {   
        for (auto &edge: mEdgeList)
            if (edge) edge->draw(target, states);

        for (auto &node: mNodeList)
            if (node) node->draw(target, states);
    }
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
    mActionQueue.pushAction(Action::DeleteNode(node, duration));
}

void DS::deleteNode(CircleNode* node)
{
    mActionQueue.pushAction([this, node](sf::Time dt) mutable -> bool
    {
        mNodeList.erase(
        std::remove_if(mNodeList.begin(), mNodeList.end(),
        [node](const CircleNode::Ptr& n) {
            return n.get() == node;
        }),
        mNodeList.end()
    );
        return true;
    });
}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration)
{
    if (!node) return;
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