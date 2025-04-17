#include <DataStructures/HeapTree.hpp>
#include <queue>
#include <iostream>
#include <cmath>

HeapTree::HeapTree() { mHeapSize = 0; }

void HeapTree::insert(int value)
{
    resetHistory();

    mCode = {
        "A.push_back(x)",
        "i = A.length - 1",
        "while i > 1 and A[i] < A[par(i)]",
        "  swap(A[i], A[par(i)]); i = par(i)",
        "finish"
    };

    mLastStep = 4;
    mLastInfo = "Inserted successfully.";

    mHeapSize++;

    CircleNode* newNode = new CircleNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    newNode->setOpacity(0);

    // createNewActionGroup();
    addNode(newNode);
    addEdge(newNode, nullptr);
    addEdge(newNode, nullptr);

    
    if (ANIMATION::Speed < 1000)
    {
        createNewActionGroup();
        mActionQueue.pushAction(Action::FadeInNode(newNode, 0.5f));
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

        sf::Vector2f curPos = par->mTargetPosition;
        if (n % 2 == 1)
            curPos += sf::Vector2f(-VIZ::DS::Size.x / (1 << (level + 1)), VIZ::DS::RowSpacing);
        else
            curPos += sf::Vector2f(VIZ::DS::Size.x / (1 << (level + 1)), VIZ::DS::RowSpacing);

        if (ANIMATION::Speed < 1000)
        {
            moveNode(newNode, curPos, 0.5f);
        }

        createNewActionGroup();
        moveEdge(par, nullptr, newNode, 0.5f);
        mActionQueue.pushInstantAction([=]() { mStep = 1; }, true);
    }
    else
    {
        newNode->mTargetPosition = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing);
        createNewActionGroup();
        newNode->setPosition(newNode->mTargetPosition);
    }

    // mActionQueue.pushInstantAction([=]() { 
        heapifyUp(n); 
    // });
}

void HeapTree::remove(int value)
{
    resetHistory();

    if (mNodeList.size() == 0) 
    {
        mLastInfo = "The heap is empty.";
        return;
    }
    

    mCode = {
        "A[0] = A[A.length - 1]",
        "i = 0; A.length--",
        "while i < A.length:",
        "  if A[i] > L := argmin(A[left], A[right])",
        "    swap(A[i], A[L]); i = L",
        "finish"
    };

    mLastStep = 5;
    mLastInfo = "Removed successfully.";

    mHeapSize--;

    int n = mNodeList.size();
    if (n > 1)
    {
        mActionQueue.pushInstantAction([=](){
            mStep = 0;
            mInfo = "Replace root with the last element.";
        });
        createNewActionGroup();
        swapTwoNodes(mNodeList[0].get(), mNodeList[n - 1].get(), 0.5f);
    }

    mActionQueue.pushInstantAction([=](){
        mStep = 1;
        mInfo = "Remove the old root.";
    });
    createNewActionGroup();
    mActionQueue.pushAction(Action::FadeOutNode(mNodeList[n - 1].get(), 0.5f));
    if (n > 1)
        moveEdge(mNodeList[(n - 2) / 2].get(), mNodeList[n - 1].get(), nullptr, 0.5f);

    createNewActionGroup();
    removeNode(mNodeList[n - 1].get());

    // mActionQueue.pushInstantAction([=]() { 
        heapifyDown(0); 
    // });
}

bool HeapTree::search(int value)
{
    resetHistory();

    mCode = {
        "for i = 0 to A.size - 1:",
        "  if A[i] == value",
        "    return i",
        "return NOT_FOUND"
    };
    
    for (int i = 0; i < mNodeList.size(); i++)
    {
        mActionQueue.pushInstantAction([=]() {
            mStep = 1;
            mInfo = "Comparing " + std::to_string(mNodeList[i]->mValue) + " with x = " + std::to_string(value) + ".";
        });
        createNewActionGroup();
        highlightNode(mNodeList[i].get(), sf::Color::Red, 0.5f);
        

        if (mNodeList[i]->mValue == value)
        {
            mActionQueue.pushInstantAction([=]() {
                mStep = 2;
                mInfo = "Found value x = " + std::to_string(value) + ".";
            });
            mLastStep = 2;

            int blinkTimes = 3;
            while (blinkTimes--)
            {
                createNewActionGroup();
                highlightNode(mNodeList[i].get(), sf::Color::Green, 0.3f);
            }
            return true;
        }
        
        mActionQueue.pushInstantAction([=]() {
            mStep = 0;
            mInfo = std::to_string(mNodeList[i]->mValue) + " is not equal to " + std::to_string(value) + " so we have to continue.";
        });
        createNewActionGroup();
        mActionQueue.pushAction(Action::Wait(0.3f));

    }

    mActionQueue.pushInstantAction([=]() {
        mStep = 3;
        mInfo = "Value x = " + std::to_string(value) + " is NOT_FOUND in the Heap.";
    });
    mLastStep = 3;
    createNewActionGroup();
    mActionQueue.pushAction(Action::Wait(0.5f));
    return false;
}

void HeapTree::updateValue(int value, int newValue)
{
    resetHistory();

    mCode = {
        "find i that A[i] == val",
        "if newVal < val: heapifyUp(i)",
        "else: heapifyDown(i)"
    };

    mLastInfo = "Updated successfully";

    mActionQueue.pushInstantAction([=](){ 
        mStep = 0; 
        mInfo = "Finding the position of val.";
    });
    for (int i = 0; i < mNodeList.size(); i++)
    {
        createNewActionGroup();
        highlightNode(mNodeList[i].get(), sf::Color::Red, 0.5f);

        if (mNodeList[i]->mValue == value)
        {
            createNewActionGroup();
            mActionQueue.pushAction(Action::ChangeNodeValue(mNodeList[i].get(), newValue, 0.5f));

            if (newValue < value)
            {
                mActionQueue.pushInstantAction([=](){ mStep = 1; });
                mLastStep = 1;
                mActionQueue.pushInstantAction([=](){ heapifyUp(i); });
            }
            else
            {
                mActionQueue.pushInstantAction([=](){ mStep = 2; });
                mLastStep = 2;
                mActionQueue.pushInstantAction([=](){ heapifyDown(i); });
            }
            
            return;
        }
    }
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

    int curValue = mNodeList[index]->mValue;
    int parValue = mNodeList[parent]->mValue;

    mActionQueue.pushInstantAction([=]() { 
        if (mLastStep == -1) mStep = 2; 
        mInfo = "Comparing " + std::to_string(curValue) + " with its parent.";
    });
    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f / 2, false));
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[parent].get(), sf::Color::Red, 0.5f / 2, false));
    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), VIZ::NODE::FillColor, 0.5f / 2, false));
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[parent].get(), VIZ::NODE::FillColor, 0.5f / 2, false));


    if (curValue < parValue)
    {
        mActionQueue.pushInstantAction([=]() { 
            if (mLastStep == -1) mStep = 3; 
            mInfo = std::to_string(curValue) + " < " + std::to_string(parValue) + " so swap them.";
        });
        swapTwoNodes(mNodeList[index].get(), mNodeList[parent].get(), 0.5f);

        // mActionQueue.pushInstantAction([=]() { 
            heapifyUp(parent); 
        // });
    }
}

void HeapTree::heapifyDown(int index)
{
    if (index >= mHeapSize) return;

    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int n = mNodeList.size();

    // createNewActionGroup();
    // mActionQueue.pushAction(Action::Wait(0.1f));

    int curValue = mNodeList[index]->mValue;

    mActionQueue.pushInstantAction([=](){
        if (mLastStep == -1) mStep = 3;
        mInfo = "Comparing " + std::to_string(curValue) + " with its children.";
    });
    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), sf::Color::Red, 0.5f / 2, false));
    if (left < mHeapSize)
        mActionQueue.pushAction(Action::HighlightNode(mNodeList[left].get(), sf::Color::Blue, 0.5f / 2, false));
    if (right < mHeapSize)
        mActionQueue.pushAction(Action::HighlightNode(mNodeList[right].get(), sf::Color::Blue, 0.5f / 2, false));
        
    createNewActionGroup();
    mActionQueue.pushAction(Action::HighlightNode(mNodeList[index].get(), VIZ::NODE::FillColor, 0.5f / 2, false));
    if (left < mHeapSize) 
        mActionQueue.pushAction(Action::HighlightNode(mNodeList[left].get(), VIZ::NODE::FillColor, 0.5f / 2, false));
    if (right < mHeapSize)
        mActionQueue.pushAction(Action::HighlightNode(mNodeList[right].get(), VIZ::NODE::FillColor, 0.5f / 2, false));

    if (left < mHeapSize && mNodeList[smallest]->mValue > mNodeList[left]->mValue)
        smallest = left;
    if (right < mHeapSize && mNodeList[smallest]->mValue > mNodeList[right]->mValue)
        smallest = right;

    int smallestValue = mNodeList[smallest]->mValue;
    if (smallest != index)
    {
        mActionQueue.pushInstantAction([=]() { 
            if (mLastStep == -1) mStep = 4; 
            mInfo = std::to_string(curValue) + " > " + std::to_string(smallestValue) + " so swap them.";
        });
        createNewActionGroup();
        swapTwoNodes(mNodeList[index].get(), mNodeList[smallest].get(), 0.5f);

        // mActionQueue.pushInstantAction([=]() { 
            heapifyDown(smallest); 
        // });
    }
}

void HeapTree::empty()
{
    DS::empty();
    mHeapSize = 0;
}