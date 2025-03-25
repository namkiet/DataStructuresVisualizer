#include <DataStructures/HeapTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

HeapTree::HeapTree():
    mRoot(nullptr)
{    
}

void HeapTree::insert(int value)
{
    TreeNode* newNode = new TreeNode(value, 16.f, sf::Color::White, sf::Color::Black);
    // newNode->mParent = prev;

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
            createNewActionGroup();
            moveEdge(par, nullptr, newNode, 0.5f);
        }
        align(0);

        // FIX
        while (n)
        {
            createNewActionGroup();
            mActionQueue.pushAction(Action::Wait(0.2f));
            // createNewActionGroup();
            // mActionQueue.pushAction(Action::HighlightNode(mNodeList[n].get(), sf::Color::Red, 0.5f));
            // mActionQueue.pushAction(Action::HighlightNode(mNodeList[(n - 1) / 2].get(), sf::Color::Red, 0.5f));

            if (mNodeList[n]->mValue < mNodeList[(n - 1) / 2]->mValue)
            {
                int parVal = mNodeList[(n - 1) / 2]->mValue;
                int curVal = mNodeList[n]->mValue;

                mNodeList[n]->mValue = parVal;
                mNodeList[(n - 1) / 2]->mValue = curVal;
    
                createNewActionGroup();
                mActionQueue.pushAction(Action::SwapNodeValues(mNodeList[n].get(), mNodeList[(n - 1) / 2].get(), 0.5f));
                align(0);
            }
            else
                break;
            
            n = (n - 1) / 2;
        }

        return true;
    });
}

void HeapTree::remove(int value)
{
    if (mNodeList.size() == 0) return;

    int n = mNodeList.size();
    if (n > 1)
        mActionQueue.pushAction(Action::SwapNodeValues(mNodeList[0].get(), mNodeList[n - 1].get(), mAnimationSpeed));

    createNewActionGroup();
    mActionQueue.pushAction(Action::FadeNode(mNodeList[n - 1].get(), mAnimationSpeed));
    if (n > 1)
        moveEdge(mNodeList[(n - 2) / 2].get(), mNodeList[n - 1].get(), nullptr, mAnimationSpeed);

    createNewActionGroup();
    removeNode(mNodeList[n - 1].get());

    createNewActionGroup();
    mActionQueue.pushAction([=](sf::Time) mutable -> bool
    {
        int i = 0;
        int n = mNodeList.size();
        while (i < n)
        {
            int largest = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && mNodeList[largest]->mValue > mNodeList[left]->mValue)
                largest = left;
            if (right < n && mNodeList[largest]->mValue > mNodeList[right]->mValue)
                largest = right;
            if (largest != i)
            {
                createNewActionGroup();
                mActionQueue.pushAction(Action::SwapNodeValues(mNodeList[i].get(), mNodeList[largest].get(), mAnimationSpeed));
                i = largest;
            }
            else
                break;
        }

        return true;
    });
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
    
    moveNode(mNodeList[index].get(), curPos, mAnimationSpeed, false);
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(2 * index + 1, leftChildPos, newSpacingX, newSpacingY);
    align(2 * index + 2, rightChildPos, newSpacingX, newSpacingY);
}