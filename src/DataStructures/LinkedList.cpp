#include <DataStructures/LinkedList.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <Core/Utility.hpp>

LinkedList::LinkedList(): mHead(nullptr) {
    maxRowNode = 8;
}

void LinkedList::insert(int value)
{
    InsertAtLast(value);
}

bool LinkedList::search(int value)
{
    if (!mHead) return false;
    
    LinkedListNode* temp = mHead;
    while (temp)
    {
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, 0.5f);
        createNewActionGroup();
        traverseEdge(temp, temp->mNext, sf::Color::Red, 0.5f);

        if (temp->mValue == value)
        {
            int blinkTimes = 3;
            while (blinkTimes--)
            {
                createNewActionGroup();
                highlightNode(temp, sf::Color::Green, 0.5f);
            }
            return true;
        }

        temp = temp->mNext;
    }
    return false;
}

void LinkedList::remove(int value)
{
    LinkedListNode* temp = mHead;
    while (temp)
    {
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, 0.5f);
        createNewActionGroup();
        traverseEdge(temp, temp->mNext, sf::Color::Red, 0.5f);

        if (temp->mValue == value)
        {
            createNewActionGroup();
            mActionQueue.pushAction(Action::MoveNode(temp, temp->getPosition() + sf::Vector2f(0, -50), 0.5f, 0));

            createNewActionGroup();
            if (temp->mPrev)
            {
                moveEdge(temp->mPrev, temp, temp->mNext, 0.5f);
                temp->mPrev->mNext = temp->mNext;
            }
            moveEdge(temp, temp->mNext, nullptr, 0.5f);

            createNewActionGroup();
            removeEdge(temp, nullptr);

            if (temp->mNext)
                temp->mNext->mPrev = temp->mPrev;

            createNewActionGroup();
            mActionQueue.pushAction(Action::FadeNode(temp, 0.5f));

            createNewActionGroup();
            removeNode(temp);
            break;
        }

        temp = temp->mNext;
    }

    align(mHead);
}




void LinkedList::InsertAtHead(int value)
{
    
    LinkedListNode* newHead = new LinkedListNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    newHead->mNext = mHead;
    newHead->mPrev = nullptr;
    newHead->setPosition(sf::Vector2f(0, 150));

    if (mHead)
        mHead->mPrev = newHead;

    createNewActionGroup();
    addNode(newHead);
    addEdge(newHead, mHead, true);

    align(newHead);
    mHead = newHead;
}

void LinkedList::InsertAtLast(int value)
{
    insertAtIndex(value, mNodeList.size());
}

void LinkedList::insertAtIndex(int value, int index)
{
    if (index == 0)
    {
        InsertAtHead(value);
        return;
    }

    LinkedListNode* cur = mHead;

    while (index--)
    {
        if (index == 0)
        {
            LinkedListNode* newNode = new LinkedListNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
            if (cur->mNext) 
                newNode->setPosition((cur->getPosition() + cur->mNext->getPosition()) / 2.f + sf::Vector2f(0, -50));

            createNewActionGroup();
            addNode(newNode);
            addEdge(newNode, nullptr, true);

            createNewActionGroup();

            newNode->mPrev = cur;

            newNode->mNext = cur->mNext;
            moveEdge(newNode, nullptr, cur->mNext, 0.5f);

            if (cur->mNext) cur->mNext->mPrev = newNode;

            moveEdge(cur, cur->mNext, newNode, 0.5f);
            cur->mNext = newNode;

        }

        createNewActionGroup();
        highlightNode(cur, sf::Color::Red, 0.5f);

        cur = cur->mNext;
    }

    align(mHead);
}

void LinkedList::align(LinkedListNode* curNode, sf::Vector2f curPos)
{
    if (!curNode) return;

    if (!curNode->mPrev) // if current node is head
    {
        curNode->mIndex = 0;
        createNewActionGroup();
    }
    else
        curNode->mIndex = curNode->mPrev->mIndex + 1;

    mActionQueue.pushAction([=](sf::Time) {
        curNode->setNote(std::to_string(curNode->mIndex));
        return true;
    });

    moveNode(curNode, curPos, 0.5f, false);
    
    int rowIndex = curNode->mIndex / maxRowNode;
    sf::Vector2f nextPos;

    if ((curNode->mIndex + 1) % maxRowNode == 0) // last node in a row
    {
        nextPos = curPos + sf::Vector2f(0, 150);
    }
    else
    {
        if (rowIndex % 2 == 0) // even row
            nextPos = curPos + sf::Vector2f(150, 0);
        else // odd row
            nextPos = curPos + sf::Vector2f(-150, 0);
    }
    
    align(curNode->mNext, nextPos);
}

void LinkedList::empty()
{
    DS::empty();
    mHead = nullptr;
}


void LinkedList::saveState() {
    // std::vector<CircleNode::Ptr> savedNodeList;
    // std::vector<Edge::Ptr> savedEdgeList;

    // if (!mRoot) {
    //     mUndoStack.push(History(std::move(savedNodeList), std::move(savedEdgeList), nullptr));
    //     std::cerr << mUndoStack.size() << "\n";
    //     return;
    // }

    // std::unordered_map<TreeNode*, TreeNode*> nodeMap;
    // std::vector<TreeNode::Ptr> tempNodeList;
    // nodeMap[nullptr] = nullptr;  // Map nullptr -> nullptr

    // // Clone root node
    // TreeNode* savedRoot = new TreeNode(*mRoot);
    // nodeMap[mRoot] = savedRoot;
    // tempNodeList.push_back(TreeNode::Ptr(savedRoot));

    // // BFS clone toàn bộ cây
    // std::queue<std::pair<TreeNode*, TreeNode*>> q;
    // q.push({mRoot, savedRoot});

    // while (!q.empty()) {
    //     auto [oldNode, newNode] = q.front();
    //     q.pop();

    //     TreeNode* newLeft = oldNode->mLeft ? new TreeNode(*oldNode->mLeft) : nullptr;
    //     TreeNode* newRight = oldNode->mRight ? new TreeNode(*oldNode->mRight) : nullptr;

    //     newNode->mLeft = newLeft;
    //     newNode->mRight = newRight;
    //     if (newLeft) newLeft->mParent = newNode;
    //     if (newRight) newRight->mParent = newNode;

    //     // nodeMap[oldNode->mLeft] = newLeft;
    //     // nodeMap[oldNode->mRight] = newRight;

    //     savedEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, newNode, newNode->mLeft, false, 1.5f));
    //     savedEdgeList.push_back(std::make_unique<Edge>(sf::Color::Black, newNode, newNode->mRight, false, 1.5f));

    //     if (newLeft) {
    //         tempNodeList.push_back(TreeNode::Ptr(newLeft));
    //         q.push({oldNode->mLeft, newLeft});
    //     }
    //     if (newRight) {
    //         tempNodeList.push_back(TreeNode::Ptr(newRight));
    //         q.push({oldNode->mRight, newRight});
    //     }
    // }

    // // Chuyển tempNodeList từ TreeNode::Ptr sang CircleNode::Ptr
    // for (auto& node : tempNodeList)
    //     savedNodeList.push_back(std::move(node));

    // TreeNode* savedBaseNode = static_cast<TreeNode*>(savedNodeList.front().get());


    // while (!mRedoStack.empty())
    //     mRedoStack.pop();

    // mUndoStack.push(History(std::move(savedNodeList), std::move(savedEdgeList), savedBaseNode));

    // std::cerr << mUndoStack.size() << "\n";
}


void LinkedList::loadState(History history)
{
    // mNodeList = std::move(history.nodeList);
    // mEdgeList = std::move(history.edgeList);
    // mRoot = static_cast<TreeNode*>(history.baseNode);
    // std::cerr << mUndoStack.size() << "\n";
}