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
    InsertAtHead(value);
}

bool LinkedList::search(int value)
{
    if (isRunning()) return false;
    DS::loadState(1.f);
    mUndoStack.clear();

    mCode = {
        "if empty: return NOT_FOUND",
        "index = 0, tmp = head",
        "while tmp.val != x:",
        "  index++, tmp = tmp.next",
        "  if tmp == null: return NOT_FOUND",
        "return index"
    };

    mLastInfo = "The whole operation is O(N).";

    if (!mHead) {
        mLastStep = 0;
        mLastInfo = "The current Linked List is empty, we return NOT_FOUND.";
        return false;
    }
    
    LinkedListNode* temp = mHead;
    while (temp)
    {
        mActionQueue.pushInstantAction([=]() {
            mStep = 2;
            mInfo = "Comparing " + std::to_string(temp->mValue) + " with x = " + std::to_string(value) + ".";
        });
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, 0.3f);

        if (temp->mValue == value)
        {
            mActionQueue.pushInstantAction([=]() {
                mStep = 5;
                mInfo = "Found value x = " + std::to_string(value) + ".";
            });
            mLastStep = 5;

            int blinkTimes = 3;
            while (blinkTimes--)
            {
                createNewActionGroup();
                highlightNode(temp, sf::Color::Green, 0.3f);
            }
            return true;
        }

        mActionQueue.pushInstantAction([=]() {
            mStep = 3;
            mInfo = std::to_string(temp->mValue) + " is not equal to " + std::to_string(value) + " so we have to continue.";
        });
        createNewActionGroup();
        traverseEdge(temp, temp->mNext, sf::Color::Red, 0.3f);

        temp = temp->mNext;
    }
    
    mActionQueue.pushInstantAction([=]() {
        mStep = 4;
        mInfo = "Value x = " + std::to_string(value) + " is NOT_FOUND in the Linked List.";
    });
    mLastStep = 4;
    createNewActionGroup();
    mActionQueue.pushAction(Action::Wait(0.5f));
    return false;
}

void LinkedList::remove(int value)
{
    if (isRunning()) return;
    DS::loadState(1.f);
    mUndoStack.clear();

    mCode = {
        "if empty: return",
        "tmp = head",
        "while tmp.val != x:",
        "  tmp = tmp.next",
        "  if tmp == null: return",
        "remove tmp"
    };

    if (!mHead)
    {
        mLastStep = 0;
        mLastInfo = "The current Linked List is empty, so we do nothing.";
        return;
    }

    LinkedListNode* temp = mHead;
    while (temp)
    {
        mActionQueue.pushInstantAction([=]() {
            mStep = 2;
            mInfo = "Comparing " + std::to_string(temp->mValue) + " with x = " + std::to_string(value) + ".";
        });
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, 0.3f);

        if (temp->mValue == value)
        {
            mActionQueue.pushInstantAction([=]() {
                mStep = 5;
                mInfo = "Remove value x = " + std::to_string(value) + ".";
            });
            mLastStep = 5;

            createNewActionGroup();
            mActionQueue.pushAction(Action::MoveNode(temp, temp->getPosition() + sf::Vector2f(0, -50), 0.5f));

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
            mActionQueue.pushAction(Action::FadeOutNode(temp, 0.5f));

            createNewActionGroup();
            removeNode(temp);

            align(mHead);
            return;
        }

        mActionQueue.pushInstantAction([=]() {
            mStep = 3;
            mInfo = std::to_string(temp->mValue) + " is not equal to " + std::to_string(value) + " so we have to continue.";
        });
        createNewActionGroup();
        traverseEdge(temp, temp->mNext, sf::Color::Red, 0.3f);

        temp = temp->mNext;
    }
    
    mActionQueue.pushInstantAction([=]() {
        mStep = 4;
        mInfo = "Value x = " + std::to_string(value) + " is NOT_FOUND in the Linked List.";
    });
    mLastStep = 4;
    createNewActionGroup();
    mActionQueue.pushAction(Action::Wait(0.5f));
}

void LinkedList::InsertAtHead(int value)
{
    if (isRunning()) return;
    DS::loadState(1.f);
    mUndoStack.clear();

    mCode = {
        "node = new Node(x)",
        "node.next = head",
        "head = node"
    };
    
    mLastStep = 2;
    mLastInfo = "The whole process is O(1).";

    LinkedListNode* newHead = new LinkedListNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    newHead->mNext = mHead;
    newHead->mPrev = nullptr;
    newHead->setPosition(sf::Vector2f(50, 0));
    newHead->setOpacity(0);

    if (mHead)
        mHead->mPrev = newHead;

    mActionQueue.pushInstantAction([=]() {
        mStep = 0;
        mInfo = "Create new vertex to store value " + std::to_string(value) + ".";
    }, false);
    mActionQueue.pushAction(Action::FadeInNode(newHead, 0.5f), false);

    createNewActionGroup();
    addNode(newHead);
    addEdge(newHead, nullptr, true);

    mActionQueue.pushInstantAction([=]() {
        mStep = 1;
        mInfo = "node.next points to the current head.";
    }, false);
    createNewActionGroup();
    moveEdge(newHead, nullptr, mHead, 0.5f);

    mActionQueue.pushInstantAction([=]() {
        mInfo = "head points to node.";
    });
    align(newHead);
    mHead = newHead;
}

void LinkedList::InsertAtLast(int value)
{
    insertAtIndex(value, mNodeList.size());
}

void LinkedList::insertAtIndex(int value, int index)
{
    if (isRunning()) return;
    DS::loadState(1.f);
    mUndoStack.clear();

    if (index == 0)
    {
        InsertAtHead(value);
        return;
    }

    bool isTailInsert = (index == mNodeList.size());

    if (index > mNodeList.size())
    {
        mLastInfo = "Index must not be greater than list size."; 
        return;
    }

    if (!isTailInsert)
    {
        mCode = {
            "pre = head",
            "for k = 0 to index - 1:",
            "  pre = pre.next",
            "node = new Node(x)",
            "node.next = pre.next; pre.next = node"
        };

        mInfo = "Find the insertion point.";
        mLastInfo = "The whole process is O(index).";
        mLastStep = mCode.size() - 1;
    }
    else
    {
        mCode = {
            "node = new Node(x)",
            "tail.next = node",
            "tail = node"
        };
    
        mLastInfo = "The whole operation is O(1) if we maintain the tail pointer.";
        mLastStep = 2;
    }

    LinkedListNode* cur = mHead;
    for (int k = 0; k < index - 1; k++)
    {
        if (!isTailInsert)
        {
            mActionQueue.pushInstantAction([=]() { mStep = 1; }, false);
            createNewActionGroup();
            highlightNode(cur, sf::Color::Red, 0.3f);

            mActionQueue.pushInstantAction([=]() { mStep = 2; }, false);
            createNewActionGroup();
            traverseEdge(cur, cur->mNext, sf::Color::Red, 0.3f);
        }
        cur = cur->mNext;
    }

    LinkedListNode* newNode = new LinkedListNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    if (cur->mNext) 
        newNode->setPosition((cur->getPosition() + cur->mNext->getPosition()) / 2.f + sf::Vector2f(0, -50));
    else
        newNode->setPosition(cur->getPosition());

    newNode->setOpacity(0);
    newNode->mIndex = cur->mIndex + 1;

    createNewActionGroup();
    addNode(newNode);
    addEdge(newNode, nullptr, true);
    
    int rowIndex = newNode->mIndex / maxRowNode;
    sf::Vector2f pos;

    if ((newNode->mIndex + 1) % maxRowNode == 0) // last node in a row
    {
        pos = cur->getPosition() + sf::Vector2f(0, 150);
    }
    else
    {
        if (rowIndex % 2 == 0) // even row
            pos = cur->getPosition() + sf::Vector2f(150, 0);
        else // odd row
            pos = cur->getPosition() + sf::Vector2f(-150, 0);
    }

    mActionQueue.pushInstantAction([=]() {
        mStep = 3;
        mInfo = "Create new vertex to store value " + std::to_string(value) + ".";
    }, false);
    mActionQueue.pushAction(Action::FadeInNode(newNode, 0.5f), false);
    moveNode(newNode, pos, 0.5f);

    if (!isTailInsert)
    {
        mActionQueue.pushInstantAction([=]() {
            mStep = 4;
            mInfo = "node.next points to pre.next, pre.next points to node.";
        });
    }
    else
    {
        mActionQueue.pushInstantAction([=]() {
            mStep = 1;
            mInfo = "tail.next points to node, tail points to node";
        });
    }

    newNode->mPrev = cur;

    moveEdge(newNode, nullptr, cur->mNext, 0.5f);
    newNode->mNext = cur->mNext;

    if (cur->mNext) cur->mNext->mPrev = newNode;

    moveEdge(cur, cur->mNext, newNode, 0.5f);
    cur->mNext = newNode;
    
    align(mHead);
}

void LinkedList::align(LinkedListNode* curNode, sf::Vector2f curPos)
{
    if (!curNode || ANIMATION::Speed >= 1000) return;

    sf::Color color;
    if (!curNode->mPrev)
        color = sf::Color(200, 50, 80);
    else if (!curNode->mNext)
        color = sf::Color(55, 90, 210);
    else
        color = VIZ::NODE::FillColor;

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

    moveNode(curNode, curPos, 0.5f);
    highlightNode(curNode, color, 0.5f, false);
    
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
    mHead = nullptr;
    DS::empty();
}

void LinkedList::saveState(std::vector<History> &stack) 
{
    if (ANIMATION::Speed >= 1000) return;

    std::vector<CircleNode::Ptr> savedNodeList;
    std::vector<Edge::Ptr> savedEdgeList;

    if (!mHead)
    {
        stack.push_back(History(std::move(savedNodeList), std::move(savedEdgeList), nullptr, mInfo, mStep));
        return;
    }

    std::vector<LinkedListNode::Ptr> tempNodeList;

    // Clone root node
    LinkedListNode* savedHead = new LinkedListNode(*mHead);
    tempNodeList.push_back(LinkedListNode::Ptr(savedHead));

    LinkedListNode* cur = mHead;
    LinkedListNode* clone = savedHead;

    while (cur) {
        LinkedListNode* nextClone = cur->mNext ? new LinkedListNode(*cur->mNext) : nullptr;

        clone->mNext = nextClone;
        if (nextClone) nextClone->mPrev = clone;

        savedEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, clone, nextClone, true, VIZ::EDGE::Thickness));
        if (nextClone) tempNodeList.push_back(LinkedListNode::Ptr(nextClone));
        
        cur = cur->mNext;
        clone = nextClone;
    }

    for (auto& node : tempNodeList)
        savedNodeList.push_back(std::move(node));

    stack.push_back(History(std::move(savedNodeList), std::move(savedEdgeList), savedHead, mInfo, mStep));
}

void LinkedList::loadState(History history)
{
    DS::loadState(std::move(history));
    mHead = static_cast<LinkedListNode*>(history.baseNode);
}