#include <DataStructures/HeapTree.hpp>
#include <queue>
#include <iostream>

HeapTree::HeapTree(): mRoot(nullptr) {}

void HeapTree::insert(int value)
{
    TreeNode* newNode = new TreeNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    newNode->setOpacity(0);

    // createNewActionGroup();
    addNode(newNode);
    addEdge(newNode, nullptr);
    addEdge(newNode, nullptr);

    
    if (ANIMATION::Speed < 1000)
    {
        createNewActionGroup();
        mActionQueue.pushAction(Action::FadeInNode(newNode, 0.5f));
    }
    
    // mActionQueue.pushInstantAction([=]()
    // {
    int n = mNodeList.size() - 1;
    if (n >= 1)
    {
        CircleNode* par = mNodeList[(n - 1) / 2].get();
        newNode->mLevel = floor(log2(n + 1));
        newNode->setPosition(par->getPosition());

        // createNewActionGroup();
        if (n % 2 == 1)
            newNode->mTargetPosition = par->mTargetPosition + sf::Vector2f(-VIZ::DS::Size.x / (1 << (newNode->mLevel + 1)), VIZ::DS::RowSpacing);
        else
            newNode->mTargetPosition = par->mTargetPosition + sf::Vector2f(VIZ::DS::Size.x / (1 << (newNode->mLevel + 1)), VIZ::DS::RowSpacing);

        if (ANIMATION::Speed < 1000)
        {
            moveNode(newNode, newNode->mTargetPosition, 0.5f);
        }

        createNewActionGroup();
        moveEdge(par, nullptr, newNode, 0.5f);
    }
    else
    {
        newNode->mTargetPosition = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing);
        createNewActionGroup();
        newNode->setPosition(newNode->mTargetPosition);
    }

    mActionQueue.pushInstantAction([=]() { heapifyUp(n); });
    // });
    
}

void HeapTree::remove(int value)
{
    if (mNodeList.size() == 0) return;

    int n = mNodeList.size();
    if (n > 1)
        swapTwoNodes(mNodeList[0].get(), mNodeList[n - 1].get());

    createNewActionGroup();
    mActionQueue.pushAction(Action::FadeOutNode(mNodeList[n - 1].get(), 0.5f));
    if (n > 1)
        moveEdge(mNodeList[(n - 2) / 2].get(), mNodeList[n - 1].get(), nullptr, 0.5f);

    createNewActionGroup();
    removeNode(mNodeList[n - 1].get());

    mActionQueue.pushInstantAction([=]() { heapifyDown(0); });
}

bool HeapTree::search(int value)
{
    return true;
}

void HeapTree::align(int index, sf::Vector2f curPos, float curSpacingX, float curSpacingY) // DFS
{
    if (index >= mNodeList.size()) return;

    if (index == 0) // is root node
        createNewActionGroup();
    
    moveNode(mNodeList[index].get(), curPos, 0.5f);
    mActionQueue.pushAction(Action::FadeInNode(mNodeList[index].get(), 0.5f));
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(2 * index + 1, leftChildPos, newSpacingX, newSpacingY);
    align(2 * index + 2, rightChildPos, newSpacingX, newSpacingY);
}

void HeapTree::heapifyUp(int index)
{
    if (index == 0) return;
    int parent = (index - 1) / 2;

    // createNewActionGroup();
    // mActionQueue.pushAction(Action::Wait(0.1f));
    
    createNewActionGroup();
    highlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f);
    highlightNode(mNodeList[parent].get(), sf::Color::Red, 0.5f);

    int curValue = mNodeList[index]->mValue;
    int parValue = mNodeList[parent]->mValue;

    if (curValue < parValue)
    {
        swapTwoNodes(mNodeList[index].get(), mNodeList[parent].get());
        mActionQueue.pushInstantAction([=]() { heapifyUp(parent); });
    }
}

void HeapTree::heapifyDown(int index)
{
    if (index >= mNodeList.size()) return;

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int n = mNodeList.size();

    // createNewActionGroup();
    // mActionQueue.pushAction(Action::Wait(0.1f));

    createNewActionGroup();
    highlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f);
    highlightNode(mNodeList[smallest].get(), sf::Color::Red, 0.5f);

    if (left < n && mNodeList[smallest]->mValue > mNodeList[left]->mValue)
        smallest = left;
    if (right < n && mNodeList[smallest]->mValue > mNodeList[right]->mValue)
        smallest = right;

    if (smallest != index)
    {
        swapTwoNodes(mNodeList[index].get(), mNodeList[smallest].get());
        createNewActionGroup();
        mActionQueue.pushInstantAction([=]() { heapifyDown(smallest); });
    }
}

void HeapTree::empty()
{
    DS::empty();
    mRoot = nullptr;
}

void HeapTree::saveState(std::stack<History> &stack)
{

}

void HeapTree::loadState(History history)
{

}