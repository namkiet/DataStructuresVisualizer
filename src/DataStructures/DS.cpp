#include <DataStructures/DS.hpp>

void DS::empty()
{
    mEdgeList.clear();
    mNodeList.clear();
    mActionQueue.empty();
}

void DS::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{    
    for (auto &edge: mEdgeList)
        if (edge) edge->draw(target, sf::Transform::Identity);

    for (auto &node: mNodeList)
        if (node) node->draw(target, states);
}

void DS::updateCurrent(sf::Time dt)   
{
    mAnimationQueue.update(dt);

    mActionQueue.update(dt);

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
void DS::addEdge(CircleNode* parent, CircleNode* child) 
{
    mActionQueue.pushAction([this, parent, child](sf::Time dt) mutable -> bool
    {
        mEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, parent, child, false));
        return true;
    });
}

Edge* DS::findEdge(CircleNode* parent, CircleNode* child) {
    for (auto& edge : mEdgeList) {
        if (edge->mFrom == parent && edge->mTo == child) {
            return edge.get();
        }
    }
    return nullptr;
}

void DS::removeEdge(CircleNode* parent, CircleNode* child) {
    mEdgeList.erase(
        std::remove_if(mEdgeList.begin(), mEdgeList.end(),
        [parent, child](const std::unique_ptr<Edge>& edge) {
            return edge->mFrom == parent && edge->mTo == child;
        }),
        mEdgeList.end()
    );
}

void DS::createNewActionGroup()
{
    mActionQueue.createNewBatch();
}

void DS::highlightNode(CircleNode* node, sf::Color highlightColor, float duration)
{

    mActionQueue.pushAction(Action::HighlightNode(node, highlightColor, duration));
    mActionQueue.pushUndo(Action::HighlightNode(node, highlightColor, duration));
}

void DS::moveNode(CircleNode* node, sf::Vector2f targetPos, float duration, bool appearEffect)
{
    if (!node) return;
    sf::Vector2f prevPos = node->getPosition();
    mActionQueue.pushAction(Action::MoveNode(node, targetPos, duration, appearEffect));
    mActionQueue.pushUndo(Action::MoveNode(node, prevPos, duration, false));
}

void DS::moveEdge(CircleNode* parent, CircleNode* child, CircleNode* targetTail, float duration)
{
    std::cerr << "HELLO\n";
    mActionQueue.pushAction(Action::MoveEdge(mEdgeList, parent, child, targetTail, duration)); 
    mActionQueue.pushUndo(Action::MoveEdge(mEdgeList, parent, targetTail, child, duration));
}

void DS::traverseEdge(CircleNode* parent, CircleNode* child, sf::Color highlightColor, float duration)
{
    mActionQueue.pushAction(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
    mActionQueue.pushUndo(Action::TraverseEdge(mEdgeList, parent, child, highlightColor, duration));
}

#include <iostream>

void DS::undo()
{
    std::cerr << "HELLO\n";
    // mActionQueue.undo();

    removeNode(mNodeList[1].get());
}

void DS::swapTwoNodes(CircleNode* a, CircleNode* b)
{
    if (!a || !b) return;

    int aVal = a->mValue;
    int bVal = b->mValue;

    TreeNode* fakeA = new TreeNode(aVal, 16.f, sf::Color::White, sf::Color::Black);
    TreeNode* fakeB = new TreeNode(bVal, 16.f, sf::Color::White, sf::Color::Black);
    fakeA->setOpacity(0);
    fakeB->setOpacity(0);
    fakeA->setPosition(a->getPosition());
    fakeB->setPosition(b->getPosition());
    addNode(fakeA);
    addNode(fakeB);

    createNewActionGroup();
    mActionQueue.pushAction([=](sf::Time) mutable -> bool {
        a->setOpacity(0);
        b->setOpacity(0);
        fakeA->setOpacity(1);
        fakeB->setOpacity(1);
        return true;
    });

    createNewActionGroup();
    moveNode(fakeA, b->getPosition(), 0.5f, false);
    moveNode(fakeB, a->getPosition(), 0.5f, false);

    createNewActionGroup();
    removeNode(fakeA);
    removeNode(fakeB);
    mActionQueue.pushAction([=](sf::Time) mutable->bool {
        a->setOpacity(1);
        a->setValue(bVal);
        b->setOpacity(1);
        b->setValue(aVal);
        return true;
    });
}

void DS::loadFromVector(std::vector<int> numList)
{
    for (int x: numList)
    {
        createNewActionGroup();
        mActionQueue.pushAction([=](sf::Time) mutable -> bool { insert(x); return true; });
    }
}