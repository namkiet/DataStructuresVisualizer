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
    TreeNode* newNode = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
    // addNode(newNode);

    mNodeList.push_back(CircleNode::Ptr(newNode));

    addEdge(newNode, nullptr);
    addEdge(newNode, nullptr);

    int n = mNodeList.size() - 1;
    if (n >= 1)
    {
        CircleNode* par = mNodeList[(n - 1) / 2].get();
        newNode->setPosition(par->getPosition());
        moveEdge(par, nullptr, newNode, 0.5f);
    }
    
    while (n)
    {
        if (mNodeList[n]->mValue < mNodeList[(n - 1) / 2]->mValue)
        {
            swap(mNodeList[n], mNodeList[(n - 1) / 2]);
            align(0);
        }
        else
            break;
        
        n = (n - 1) / 2;
    }

    align(0);
}

void HeapTree::remove(int value)
{
    // mRoot = remove(mRoot, value);
}

bool HeapTree::search(int value)
{
    return true;
}

void HeapTree::align(int index, sf::Vector2f curPos, float curSpacingX, float curSpacingY) // DFS
{
    if (index >= mNodeList.size()) return;

    if (index == 0) // is root node
    {
        // curNode->mLevel = 0;
        createNewActionGroup();
    }
    // else
    //     curNode->mLevel = curNode->mParent->mLevel + 1;
    
    moveNode(mNodeList[index].get(), curPos, mAnimationSpeed, false);
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(2 * index + 1, leftChildPos, newSpacingX, newSpacingY);
    align(2 * index + 2, rightChildPos, newSpacingX, newSpacingY);
}