#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>
#include <iostream>

DS::DS() { empty(); stop = false; playback = NORMAL; timer = 0;}

void DS::empty()
{
    mCode.clear();
    mLastStep = -1;
    mInfo = "";
    mLastInfo = "";

    mH.clear();
    keyFrames.clear();
    cS = -1;
    stop = isStepByStep;

    mEdgeList.clear();
    mNodeList.clear();
    mActionQueue.empty();
}

bool DS::isRunning() { return !mActionQueue.isEmpty(); }

void DS::saveStep()
{
    if (ANIMATION::Speed >= 1000) return;

    cS++;

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
    rt.setSmooth(true);
    mH.push_back(rt.getTexture());
}

void DS::loadStep(float progress)
{
    if (mH.empty()) return;
    cS = int(progress * (mH.size() - 1));
}

bool DS::canUndo() { 
    return cS > 0; 
}

bool DS::canRedo() {
    return cS + 1 < mH.size(); 
}

void DS::undo()
{
    if (!canUndo()) return;
    playback = UNDO;
    stop = true;

    if (!keyFrames.empty() && cS > keyFrames.back())
    {
        targetFrame = keyFrames.back();
        return;
    }

    for (int i = 1; i < keyFrames.size(); i++)
    {
        if (keyFrames[i - 1] < cS && cS <= keyFrames[i])
        {
            targetFrame = keyFrames[i - 1];
            return;
        }
    }
}

void DS::redo()
{
    if (!canRedo()) return;
    playback = REDO;
    stop = true;

    if (keyFrames.empty() || cS >= keyFrames.back())
    {
        targetFrame = mH.size() - 1;
        return;
    }

    for (int i = 1; i < keyFrames.size(); i++)
    {
        if (keyFrames[i - 1] <= cS && cS < keyFrames[i])
        {
            targetFrame = keyFrames[i];
            return;
        }
    }
}

void DS::toFirst()
{
    if (!canUndo()) return;
    playback = UNDO;
    stop = true;
    cS = targetFrame = 0;
}

void DS::toLast()
{
    if (!canRedo()) return;
    playback = REDO;
    stop = true;
    cS = targetFrame = mH.size() - 1;
}

void DS::run()
{
    stop = !stop;
}

float DS::getProgress()
{
    if (isRunning() && playback == NORMAL) return elapsedTimer / totalTimer;

    if (mH.size() <= 1) return mH.size();
    float progress = (cS * 1.f) / (mH.size() - 1) * (elapsedTimer == 0 ? 1 : elapsedTimer / totalTimer);
    return progress;
}

void DS::resetHistory()
{
    mCode.clear();
    mLastStep = -1;
    mInfo = "";
    mLastInfo = "";

    mH.clear();
    keyFrames.clear();
    cS = -1;

    stop = isStepByStep;

    saveStep();

    keyFrames.push_back(0);

    playback = NORMAL;

}

void DS::updateCurrent(sf::Time dt)   
{   
    if (playback == UNDO)
    {
        timer += dt.asSeconds();
        if (timer >= 0.08f)
        {
            std::cerr << cS << " " << targetFrame << "\n";
            if (cS > targetFrame) cS--;
            if (!stop) 
            {
                redo();
                stop = false;
            }
            timer = 0;
        }
        return;
    }
    
    if (playback == REDO)
    { 
        timer += dt.asSeconds();
        if (timer >= 0.08f)
        {
            if (cS < targetFrame) cS++;
            if (cS == mH.size() - 1) playback = NORMAL;
            if (!stop) 
            {
                redo();
                stop = false;
            }
            timer = 0;
        }
        return;
    }
    
    // if (totalTimer)
    // else
    //     elapsedTimer = 0;


    if (mActionQueue.isEmpty())
    {
        if (ANIMATION::Speed < 1000)
        {
            mStep = mLastStep;
            if (mLastInfo != "#") mInfo = mLastInfo;
        }

        totalTimer = 0;
        elapsedTimer = 0;
    }
    else
    {
        if (stop) return;

        totalTimer = std::max(totalTimer, mActionQueue.getTotalTime());

        float t = mActionQueue.update(dt);
        timer += ANIMATION::Speed * dt.asSeconds();

        if (t > 0 || timer >= std::max(0.1f, totalTimer / 100))
            saveStep();

        if (t > 0)
        {
            keyFrames.push_back(mH.size() - 1);
            if (isStepByStep) stop = true;
        }

        if (t == -1)
        {
            elapsedTimer += ANIMATION::Speed * dt.asSeconds();
        }

        if (timer >= std::max(0.1f, totalTimer / 100)) timer = 0;
    }

    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);

    for (auto &node: mNodeList)
        if (node) node->update(dt);

}

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    states.transform *= getTransform();  

    if (cS != mH.size() - 1 && !mH.empty() && cS != -1) // not the last step
    {
        sf::Sprite sprite(mH[cS]);
        sprite.setColor(sf::Color(220, 220, 220, 200));
        target.draw(sprite, states);
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
void DS::addEdge(CircleNode* parent, CircleNode* child, int weight, bool hasArrow) 
{
    if (weight == -1) // unweighted
        mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, hasArrow, VIZ::EDGE::Thickness));
    else // weighted
        mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, weight, hasArrow, VIZ::EDGE::Thickness));
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
    if (reverse)
    {
        sf::Color oldColor = node->getFillColor();
        mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration / 2, false));
        createNewActionGroup();
        mActionQueue.pushAction(Action::HighlightNode(node, oldColor, duration / 2, false));
    }
    else
        mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration, false));
}

void DS::changeNodeColor(CircleNode* node, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::ChangeNodeColor(node, highlightColor, duration));
}

void DS::deleteNodeEffect(CircleNode* node, float duration) // remove node from mNodeList and create dissapear effect
{
    mActionQueue.pushAction(Action::DeleteNode(node, duration));
}

void DS::deleteNode(CircleNode* node)
{
    mActionQueue.pushInstantAction([=]()
    {
        mNodeList.erase(
            std::remove_if(mNodeList.begin(), mNodeList.end(),
            [node](const CircleNode::Ptr& n) {
                return n.get() == node;
            }),
            mNodeList.end()
        );
    });
}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration)
{
    if (!node) return;
    node->mTargetPosition = targetPos;
    mActionQueue.pushAction(Action::MoveNode(node, targetPos, duration));
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
{
    mActionQueue.pushAction(Action::MoveEdge(mEdgeList, parent, child, targetTail, duration)); 
}

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration, bool reverse)
{
    if (reverse)
    {
    // createNewActionGroup();
        mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration / 2, false));
        createNewActionGroup();
        mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, VIZ::EDGE::Color, duration / 2, false));
    }
    else
    {
        mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration, false));
    }
}
void DS::changeEdgeColor(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::ChangeEdgeColor(mEdgeList,parent, child, highlightColor, duration));
}

void DS::swapTwoNodes(CircleNode* a, CircleNode* b, float duration)
{
    if (!a || !b) return;

    int aVal = a->mValue;
    int bVal = b->mValue;

    a->mValue = bVal;
    b->mValue = aVal;

    // Create 2 fake nodes
    TreeNode* fakeA = new TreeNode(aVal, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    TreeNode* fakeB = new TreeNode(bVal, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    fakeA->setOpacity(0);
    fakeB->setOpacity(0);
    fakeA->setPosition(a->mTargetPosition);
    fakeB->setPosition(b->mTargetPosition);
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
    moveNode(fakeA, b->mTargetPosition, duration);
    moveNode(fakeB, a->mTargetPosition, duration);

    // Remove the 2 fake nodes
    createNewActionGroup();
    removeNode(fakeA);
    removeNode(fakeB);

    // Make the real nodes appear again
    mActionQueue.pushInstantAction([=]() {
        a->setOpacity(1);
        b->setOpacity(1);
        a->setValue(bVal);
        b->setValue(aVal);
    });
}

bool DS::loadFromVector(std::vector<int> numList)
{
    if(numList.size() > 50) return false;

    empty();
    bool oldStepByStep = isStepByStep;
    isStepByStep = stop = false;
    playback = NORMAL;
    auto curSpeed = ANIMATION::Speed;
    ANIMATION::Speed = 1000;
    for (int x: numList) insert(x);
    resetHistory(); 
    
    mActionQueue.pushInstantAction([=]()
    { 
        ANIMATION::Speed = curSpeed;
        align(); 
    });
    mActionQueue.pushInstantAction([=]() { 
        resetHistory(); 
        isStepByStep = stop = oldStepByStep; 
    }); 

    return true;
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

sf::Vector2f DS::getWorldPosition(int id){
    return mNodeList[id]->getPosition() + VIZ::DS::Position;
}

void DS::changeNodePosition(int id, sf::Vector2f pos)
{
    mNodeList[id]->setPosition(pos);
    for(auto& edge: mEdgeList){
        edge->updateEdge();
    }
}

int DS::getSize() const
{
    return mNodeList.size();
}