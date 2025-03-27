#include <DataStructures/HeapTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

HeapTree::HeapTree():
    mRoot(nullptr)
{    
}

// void HeapTree::rawInsert(int value)
// {
//     mNodeList.push_back(new TreeNode(value, 16.f, sf::Color::White, sf::Color::Black));
// }

void HeapTree::insert(int value)
{
    TreeNode* newNode = new TreeNode(value, 16.f, sf::Color::White, sf::Color::Black);

    createNewActionGroup();
    addNode(newNode);
    addEdge(newNode, nullptr);
    addEdge(newNode, nullptr);
    
    createNewActionGroup();
    mActionQueue.pushAction([=](sf::Time) mutable->bool {
        int n = mNodeList.size() - 1;
        if (n >= 1)
        {
            CircleNode* par = mNodeList[(n - 1) / 2].get();
            newNode->setPosition(par->getPosition());

            newNode->setOpacity(0);
            newNode->mLevel = floor(log2(n + 1));

            createNewActionGroup();
            if (n % 2 == 1)
                newNode->mTargetPostion = par->mTargetPostion + sf::Vector2f(-mMaxWidth / (1 << (newNode->mLevel + 1)), mVerticalSpacing);
            else
                newNode->mTargetPostion = par->mTargetPostion + sf::Vector2f(mMaxWidth / (1 << (newNode->mLevel + 1)), mVerticalSpacing);

            moveNode(newNode, newNode->mTargetPostion, 0.5f, true);

            createNewActionGroup();
            moveEdge(par, nullptr, newNode, 0.5f);
        }
        else
        {
            newNode->mTargetPostion = sf::Vector2f(600, 100);
            newNode->setPosition(sf::Vector2f(600, 100));
        }

        createNewActionGroup();
        mActionQueue.pushAction([=](sf::Time) mutable -> bool { heapifyUp(n); return true; });
        return true;
    });
    
}

void HeapTree::remove(int value)
{
    if (mNodeList.size() == 0) return;

    int n = mNodeList.size();
    if (n > 1)
        swapTwoNodes(mNodeList[0].get(), mNodeList[n - 1].get());
        // mActionQueue.pushAction(Action::SwapNodeValues(mNodeList[0].get(), mNodeList[n - 1].get(), 0.5f));

    createNewActionGroup();
    mActionQueue.pushAction(Action::FadeNode(mNodeList[n - 1].get(), 0.5f));
    if (n > 1)
        moveEdge(mNodeList[(n - 2) / 2].get(), mNodeList[n - 1].get(), nullptr, 0.5f);

    createNewActionGroup();
    removeNode(mNodeList[n - 1].get());

    createNewActionGroup();
    mActionQueue.pushAction([=](sf::Time) mutable -> bool { heapifyDown(0); return true; });
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
    
    moveNode(mNodeList[index].get(), curPos, 0.5f, false);
    
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

    createNewActionGroup();
    mActionQueue.pushAction(Action::Wait(0.2f));
    
    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f));
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[parent].get(), sf::Color::Red, 0.5f));

    int curValue = mNodeList[index]->mValue;
    int parValue = mNodeList[parent]->mValue;

    if (curValue < parValue)
    {
        swapTwoNodes(mNodeList[index].get(), mNodeList[parent].get());
        createNewActionGroup();
        mActionQueue.pushAction([=](sf::Time){ heapifyUp(parent); return true; });
    }
}

void HeapTree::heapifyDown(int index)
{
    if (index >= mNodeList.size()) return;

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int n = mNodeList.size();

    createNewActionGroup();
    mActionQueue.pushAction(Action::Wait(0.2f));

    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f));
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[smallest].get(), sf::Color::Red, 0.5f));

    if (left < n && mNodeList[smallest]->mValue > mNodeList[left]->mValue)
        smallest = left;
    if (right < n && mNodeList[smallest]->mValue > mNodeList[right]->mValue)
        smallest = right;

    if (smallest != index)
    {
        swapTwoNodes(mNodeList[index].get(), mNodeList[smallest].get());
        createNewActionGroup();
        mActionQueue.pushAction([=](sf::Time){ heapifyDown(smallest); return true; });
    }
}

void HeapTree::empty()
{
    DS::empty();
    mRoot = nullptr;
}