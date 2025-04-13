#include <DataStructures/HeapTree.hpp>
#include <queue>
#include <iostream>

HeapTree::HeapTree() {}

void HeapTree::insert(int value)
{
    execute();

    mCode = {
        "A.push_back(x)",
        "i = A.length - 1",
        "while i > 1 and A[i] < A[par(i)]",
        "  swap(A[i], A[par(i)]); i = par(i)",
        "finish"
    };

    mLastStep = 4;
    mLastInfo = "Inserted successfully.";

    TreeNode* newNode = new TreeNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    newNode->setOpacity(0);

    // createNewActionGroup();
    addNode(newNode);
    addEdge(newNode, nullptr);
    addEdge(newNode, nullptr);
    
    if (ANIMATION::Speed < 1000)
    {
        createNewActionGroup();
        mActionQueue.pushAction(Action::FadeInNode(newNode, 0.3f));
        mActionQueue.pushInstantAction([=]() { 
            mStep = 0; 
            mInfo = std::to_string(value) + " is inserted at the back of the array A.";
        }, true);
    }
    
    int n = mNodeList.size() - 1;
    if (n >= 1)
    {
        CircleNode* par = mNodeList[(n - 1) / 2].get();
        int level = floor(log2(n + 1));
        newNode->setPosition(par->getPosition());

        if (n % 2 == 1)
            newNode->mTargetPosition = par->mTargetPosition + sf::Vector2f(-VIZ::DS::Size.x / (1 << (level + 1)), VIZ::DS::RowSpacing);
        else
            newNode->mTargetPosition = par->mTargetPosition + sf::Vector2f(VIZ::DS::Size.x / (1 << (level + 1)), VIZ::DS::RowSpacing);

        if (ANIMATION::Speed < 1000)
        {
            moveNode(newNode, newNode->mTargetPosition, 0.3f);
        }

        createNewActionGroup();
        moveEdge(par, nullptr, newNode, 0.3f);
        mActionQueue.pushInstantAction([=]() { mStep = 1; }, true);
    }
    else
    {
        newNode->mTargetPosition = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing);
        createNewActionGroup();
        newNode->setPosition(newNode->mTargetPosition);
    }

    mActionQueue.pushInstantAction([=]() { heapifyUp(n); });
}

void HeapTree::remove(int value)
{
    execute();

    if (mNodeList.size() == 0) return;

    int n = mNodeList.size();
    if (n > 1)
        swapTwoNodes(mNodeList[0].get(), mNodeList[n - 1].get(), 0.3f);

    createNewActionGroup();
    mActionQueue.pushAction(Action::FadeOutNode(mNodeList[n - 1].get(), 0.3f));
    if (n > 1)
        moveEdge(mNodeList[(n - 2) / 2].get(), mNodeList[n - 1].get(), nullptr, 0.3f);

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
    
    moveNode(mNodeList[index].get(), curPos, 0.3f);
    mActionQueue.pushAction(Action::FadeInNode(mNodeList[index].get(), 0.3f));
    
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

    execute();
    int parent = (index - 1) / 2;

    // createNewActionGroup();
    // mActionQueue.pushAction(Action::Wait(0.1f));

    int curValue = mNodeList[index]->mValue;
    int parValue = mNodeList[parent]->mValue;
    
    createNewActionGroup();
    highlightNode(mNodeList[index].get(), sf::Color::Red, 0.3f);
    highlightNode(mNodeList[parent].get(), sf::Color::Red, 0.3f);
    mActionQueue.pushInstantAction([=]() { 
        mStep = 2; 
        mInfo = "Comparing " + std::to_string(curValue) + " with its parent.";
    }, true);

    if (curValue < parValue)
    {
        mActionQueue.pushInstantAction([=]() { 
            mStep = 3; 
            mInfo = std::to_string(curValue) + " < " + std::to_string(parValue) + " so swap them.";
        });
        swapTwoNodes(mNodeList[index].get(), mNodeList[parent].get(), 0.3f);

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
    highlightNode(mNodeList[index].get(), sf::Color::Red, 0.3f);
    highlightNode(mNodeList[smallest].get(), sf::Color::Red, 0.3f);

    if (left < n && mNodeList[smallest]->mValue > mNodeList[left]->mValue)
        smallest = left;
    if (right < n && mNodeList[smallest]->mValue > mNodeList[right]->mValue)
        smallest = right;

    if (smallest != index)
    {
        createNewActionGroup();
        swapTwoNodes(mNodeList[index].get(), mNodeList[smallest].get(), 0.3f);

        mActionQueue.pushInstantAction([=]() { heapifyDown(smallest); });
    }
}

void HeapTree::empty()
{
    DS::empty();
}

void HeapTree::saveState(std::vector<History> &stack)
{
    if (ANIMATION::Speed >= 1000) return;

    std::vector<CircleNode::Ptr> savedNodeList;
    std::vector<Edge::Ptr> savedEdgeList;

    int n = mNodeList.size();

    for (int i = 0; i < mNodeList.size(); i++)
    {
        CircleNode* cur = new CircleNode(*mNodeList[i].get());
        savedNodeList.push_back(CircleNode::Ptr(cur));

        CircleNode* left = 2 * i + 1 < n ? new CircleNode(*mNodeList[2 * i + 1].get()) : nullptr;
        CircleNode* right = 2 * i + 2 < n ? new CircleNode(*mNodeList[2 * i + 2].get()) : nullptr;
        savedEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, cur, left, false, VIZ::EDGE::Thickness));
        savedEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, cur, right, false, VIZ::EDGE::Thickness));
    }

    stack.push_back(History(std::move(savedNodeList), std::move(savedEdgeList), nullptr, mInfo, mStep));
}

void HeapTree::loadState(History history)
{
    std::cerr << "LoadHeap \n";
    DS::loadState(std::move(history));
}