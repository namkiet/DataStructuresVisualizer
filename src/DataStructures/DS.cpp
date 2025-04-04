#include <DataStructures/DS.hpp>
#include <Core/Variables.hpp>

void DS::empty()
{
    mEdgeList.clear();
    mNodeList.clear();
    mActionQueue.empty();
}

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    states.transform *= getTransform();  
    
    for (auto &edge: mEdgeList)
        if (edge) edge->draw(target, states);

    for (auto &node: mNodeList)
        if (node) node->draw(target, states);
}

void DS::updateCurrent(sf::Time dt)   
{
    mActionQueue.update(dt);

    if (mActionQueue.isEmpty())
        mStep = mCode.size() - 1;

    for (auto &edge: mEdgeList)
        if (edge) edge->update(dt);

    for (auto &node: mNodeList)
        if (node) node->update(dt);

}

void DS::addNode(CircleNode* node)
{
    mActionQueue.pushAction([this, node](sf::Time dt) mutable -> bool
    {
        mNodeList.push_back(CircleNode::Ptr(node));
        return true;
    });
}

void DS::removeNode(CircleNode* node)
{
    mActionQueue.pushAction([this, node](sf::Time dt) mutable->bool
    {
        mEdgeList.erase(
            std::remove_if(mEdgeList.begin(), mEdgeList.end(),
                [node](const std::unique_ptr<Edge>& edge) {
                    return edge->mFrom == node;
                }),
            mEdgeList.end()
        );

        mNodeList.erase(
            std::remove_if(mNodeList.begin(), mNodeList.end(),
            [node](const CircleNode::Ptr& ptr) { return ptr.get() == node; }),
            mNodeList.end()
        );
        return true;
    });
}
void DS::addEdge(CircleNode* parent, CircleNode* child, bool hasArrow) 
{
    mActionQueue.pushAction([this, parent, child, hasArrow](sf::Time dt) mutable -> bool
    {
        mEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, parent, child, hasArrow, VIZ::EDGE::Thickness));
        return true;
    });
}

void DS::removeEdge(CircleNode* parent, CircleNode* child) {
    mActionQueue.pushAction([this, parent, child](sf::Time dt) mutable -> bool
    {
        mEdgeList.erase(
            std::remove_if(mEdgeList.begin(), mEdgeList.end(),
            [parent, child](const std::unique_ptr<Edge>& edge) {
                return edge->mFrom == parent && edge->mTo == child;
            }),
            mEdgeList.end()
        );
        std::cout<<"Edge removed\n";
        return true;
    });
}

void DS::createNewActionGroup()
{
    mActionQueue.createNewBatch();
}

void DS::highlightNode(CircleNode* node, sf::Color highlightColor, float duration)
{

    mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration));
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

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect)
{
    if (!node) return;
    sf::Vector2f prevPos = node->getPosition();
    mActionQueue.pushAction(Action::MoveNode(node, targetPos, duration, appearEffect));
    std::cout<<"1 here"<<std::endl;
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
{
    mActionQueue.pushAction(Action::MoveEdge(mEdgeList, parent, child, targetTail, duration)); 
}

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
}

void DS::swapTwoNodes(CircleNode* a, CircleNode* b)
{
    if (!a || !b) return;

    int aVal = a->mValue;
    int bVal = b->mValue;

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
    moveNode(fakeA, b->getPosition(), 0.5f, false);
    moveNode(fakeB, a->getPosition(), 0.5f, false);

    // Remove the 2 fake nodes
    createNewActionGroup();
    removeNode(fakeA);
    removeNode(fakeB);

    // Make the real nodes appear again
    mActionQueue.pushInstantAction([=]() {
        a->setOpacity(1);
        a->setValue(bVal);
        b->setOpacity(1);
        b->setValue(aVal);
    });
}

void DS::loadFromVector(std::vector<int> numList)
{
    empty();
    auto curSpeed = ANIMATION::Speed;
    ANIMATION::Speed = 1000;
    for (int x: numList) insert(x);
    mActionQueue.pushInstantAction([=](){ ANIMATION::Speed = curSpeed; });
}    

bool DS::canUndo()
{
    return !mUndoStack.empty();
}

bool DS::canRedo()
{
    return !mRedoStack.empty();
}


void DS::undo()
{
    if (!canUndo()) return;
    
    empty();
    History history = std::move(mUndoStack.top());
    mUndoStack.pop();

    loadState(std::move(history));
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