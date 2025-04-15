#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>
#include <iostream>

DS::DS() { resetHistory(); mActionCount = 0; stop = true; }

void DS::empty()
{
    mCode.clear();
    mInfo = "";
    mLastInfo = "";

    mEdgeList.clear();
    mNodeList.clear();
    mActionQueue.empty();
}

bool DS::isRunning() { return !mActionQueue.isEmpty(); }

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

bool DS::canUndo() { 
    return cS > 0; 
}

bool DS::canRedo() {
    return cS + 1 < mH.size(); 
}

void DS::undo()
{
    // if (canUndo()) cS--;
    // if (keyID < 0) return;

    // cS = keyFrames[keyID];
    // keyID--;

    if (!canUndo()) return;

    for (int i = 1; i < keyFrames.size(); i++)
    {
        if (keyFrames[i - 1] < cS && cS <= keyFrames[i])
        {
            cS = keyFrames[i - 1];
            return;
        }
    }
}

void DS::redo()
{
    // stop = false;
    // return;
    // if (canRedo()) cS++;

    if (!canRedo()) return;

    for (int i = 1; i < keyFrames.size(); i++)
    {
        if (keyFrames[i - 1] <= cS && cS < keyFrames[i])
        {
            cS = keyFrames[i];
            return;
        }
    }
}

void DS::sBs()
{
    stop = false;
}

float DS::getProgress()
{
    if (isRunning())
    {
        if (mActionCount <= 0) return 1.f;
        float progress = (mCurrentAction * 1.f) / mActionCount;
        return progress;
    }

    if (mH.size() <= 1) return 1.f;
    float progress = (cS * 1.f) / (mH.size() - 1);
    return progress;
}

void DS::resetHistory()
{
    mCode.clear();

    mLastStep = -1;
    mInfo = "";
    mLastInfo = "";

    // auto c = mH.back();
    mH.clear();
    keyFrames.clear();

    saveStep();
    // mH.push_back(c);
    cS = 0;

    keyFrames.push_back(0);

    keyID = -1;

    stop = true;
}

void DS::updateCurrent(sf::Time dt)   
{   
    if (isStepByStep)
    { 
        if (!stop)
        {
            float t = mActionQueue.update(dt);

            if (t > 0) stop = true;
            
            if (mActionQueue.isEmpty())
            {
                mStep = mLastStep;
                if (mLastInfo != "#") mInfo = mLastInfo;
                
                mCurrentAction = 0;
                mActionCount = 0;
            }
        }

        

        for (auto &edge: mEdgeList)
            if (edge) edge->update(dt);

        for (auto &node: mNodeList)
            if (node) node->update(dt);
        
        return;
}

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
        
        mCurrentAction = 0;
        mActionCount = 0;
    }
    else 
    {
        mActionCount = std::max(mActionCount, mActionQueue.size());

        timer += ANIMATION::Speed / mActionCount;

        // if (t)
        // {
        float t = mActionQueue.update(dt);


        // if (cS == mH.size() - 1) cS++;
        // saveStep();
        
        // keyID++;
        // keyFrames.push_back(mH.size() - 1);

        // // }
        // // keyFrames.push_back(cS);
        // if (t >= 0) 
        //     mCurrentAction++;

        // if (t <= 0 && timer <= 0.1f) // only update every 0.1s or before important frames
        // {
        //     if (cS == mH.size() - 1) cS--;
        //     mH.pop_back();
        // }

        // if (t <= 0)
        // {
        //     keyID--;
        //     keyFrames.pop_back();
        // }

        // if (t) // important frame
        // {
        //     saveStep();
        //     cS++;
        //     // keyFrames.push_back(cS);
        //     // keyID++;
        // }

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

    if (cS != mH.size() - 1 && !mH.empty()) // not the last step
    {
        sf::Sprite sprite(mH[cS]);
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
    if(weight == -1)
    {
        mActionQueue.pushAction([this, parent, child, hasArrow](sf::Time dt) mutable -> bool
        {
            mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, hasArrow, VIZ::EDGE::Thickness));
            return true;
        });
    }
    //no Weight

    else
    {
        std::cout<<"Go here ok"<<std::endl;
        mActionQueue.pushAction([this, parent, child, hasArrow,weight](sf::Time dt) mutable -> bool
    {
        mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, weight, hasArrow, VIZ::EDGE::Thickness));
        return true;
    });
    } // has Weight   
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
        mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration / 2, false));
        createNewActionGroup();
        mActionQueue.pushAction(Action::HighlightNode(node, VIZ::NODE::FillColor, duration / 2, false));
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

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration, bool reverse)
{
    createNewActionGroup();
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration, false));
    createNewActionGroup();
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, VIZ::EDGE::Color, duration, false));
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