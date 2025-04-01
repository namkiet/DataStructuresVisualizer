#include <DataStructures/LinkedList.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>


LinkedList::LinkedList(): mHead(nullptr) {
    maxRowNode = 5;
}


void LinkedList::InsertAtHead(int value)
{
    
    LinkedListNode* newHead = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    // newHead->mTargetPosition = sf::Vector2f(400, 550);
    newHead->setPosition(sf::Vector2f(0, 100));
    newHead->setNext(mHead);
    newHead->setOpacity(1);
    newHead->setPrev(nullptr);

    if (mHead)
        mHead->setPrev(newHead);
    
    // mHead = newHead;
    // createNewActionGroup();
    // addNode(mHead);
    // highlightNode(mHead, sf::Color::Red, mAnimationSpeed);

    // createNewActionGroup();
    // LinkedListNode* temp = mHead->mNext;
    // while(temp != NULL)
    // {
    //     temp->mTargetPosition += sf::Vector2f(100, 0);
    //     moveNode(temp, temp->mTargetPosition, mAnimationSpeed, true);
    //     temp = temp->mNext;
    // }
    // createNewActionGroup();
    // moveNode(mHead, sf::Vector2f(450,400), mAnimationSpeed, true);
    // addEdge(mHead, mHead->mNext, true);

    createNewActionGroup();
    addNode(newHead);
    addEdge(newHead, mHead, true);

    align(newHead);

    mHead = newHead;
}

void LinkedList::InsertAtLast(int value)
{
    insertAtIndex(value, 3);
    // if (!mHead)
    // {
    //     InsertAtHead(value);
    //     return;
    // }

    // LinkedListNode* newLast = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    // newLast->mTargetPosition = sf::Vector2f(0, 0);
    // newLast->setPosition(newLast->mTargetPosition);
    // newLast->setNext(nullptr);

    // createNewActionGroup();
    // addNode(newLast);
    // // highlightNode(newLast, sf::Color::Red, mAnimationSpeed);

    // LinkedListNode* temp = mHead;
    // while (temp->mNext)
    // {
    //     createNewActionGroup();
    //     highlightNode(temp, sf::Color::Red, mAnimationSpeed);
    //     createNewActionGroup();
    //     traverseEdge(temp, temp->mNext, sf::Color::Red, mAnimationSpeed);
    //     temp = temp->mNext;
    // }

    // // createNewActionGroup();
    // // highlightNode(temp1, sf::Color::Red, mAnimationSpeed);

    // temp->setNext(newLast);
    // newLast->setPrev(temp);

    // createNewActionGroup();

    // addEdge(temp, newLast, true);

    // align(mHead);

    // newLast->mTargetPosition = temp1->mTargetPosition + sf::Vector2f(100, 0);
    // moveNode(newLast, newLast->mTargetPosition, mAnimationSpeed, true);
}

void LinkedList::insertAtIndex(int value, int index)
{
    LinkedListNode* cur = mHead;

    while (index--)
    {
        // if (!cur)
        // {
            // if (index != 0) break;
        if (index == 0)
        {
            LinkedListNode* newNode = new LinkedListNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
            newNode->setPosition(sf::Vector2f(0, 0));

            createNewActionGroup();
            addNode(newNode);
            addEdge(newNode, nullptr, true);
            // exit(0);

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

void LinkedList::insert(int value)
{
    InsertAtLast(value);
}

bool LinkedList::search(int value)
{
    if (!mHead)
    {
        std::cout<<"Not found "<<value<<std::endl;
        return true;
    }
    LinkedListNode* temp = mHead;
    while(temp)
    {
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, mAnimationSpeed);
        if (temp->mValue == value)
        {
            createNewActionGroup();
            highlightNode(temp, sf::Color::Green, mAnimationSpeed*0.5);
            createNewActionGroup();
            highlightNode(temp, sf::Color::Green, mAnimationSpeed*0.5);
            std::cout<<"Found "<<value<<std::endl;
            return true;
        }
        createNewActionGroup();
        traverseEdge(temp, temp->mNext, sf::Color::Red, mAnimationSpeed);
        temp = temp->mNext;
    }
    std::cout<<"Not found "<<value<<std::endl;
    return true;
}
void LinkedList::remove(int value)
{
    int a = search(value);
    if (!mHead)
    {
        std::cout<<"Not found "<<value<<std::endl;
        return;
    }
    std::cout<<"Delete "<<value<<std::endl;
    LinkedListNode* temp = mHead;
    if (temp->mValue == value)
    {
        std::cout<<"Head"<<std::endl;
        mHead = temp->mNext;
        if (mHead)
        {
            mHead->setPrev(nullptr);
        }
        temp->mTargetPosition += sf::Vector2f(0, 100);
        createNewActionGroup();
        moveNode(temp, temp->mTargetPosition, mAnimationSpeed, true);
        highlightNode(temp, sf::Color::Red, mAnimationSpeed);
        traverseEdge(temp, temp->mNext, sf::Color::Red, mAnimationSpeed);

        createNewActionGroup();
        removeEdge(temp, temp->mNext);

        createNewActionGroup();
        deleteNodeEffect(temp, mAnimationSpeed);
        
        LinkedListNode* temp1 = mHead;
        while(temp1)
        {
            std::cout<<"Start loop"<<std::endl;
            temp1->mTargetPosition += sf::Vector2f(-100, 0);
            moveNode(temp1, temp1->mTargetPosition, mAnimationSpeed, true);
            temp1 = temp1->mNext;
            std::cout<<"end loop"<<std::endl;
        }

        createNewActionGroup();
        deleteNode(temp);
    }
    else {
        while(temp->mNext)
        {
            if (temp->mNext->mValue == value)
            {
                    std::cout<<"Not head"<<std::endl;
                createNewActionGroup();
                highlightNode(temp->mNext, sf::Color::Red, mAnimationSpeed);
                traverseEdge(temp, temp->mNext, sf::Color::Red, mAnimationSpeed);
                traverseEdge(temp->mNext, temp->mNext->mNext, sf::Color::Red, mAnimationSpeed);
                    std::cout<<"OK here"<<std::endl;
                createNewActionGroup();
                removeEdge(temp, temp->mNext);
                removeEdge(temp->mNext, temp->mNext->mNext);
                createNewActionGroup();
                temp->mNext->mTargetPosition += temp->mNext->mTargetPosition + sf::Vector2f(0.f, 100.f);
                moveNode(temp->mNext, temp->mNext->mTargetPosition, mAnimationSpeed, true);
                    std::cout<<"OK1 here"<<std::endl;
                createNewActionGroup();
                deleteNodeEffect(temp->mNext, mAnimationSpeed);

                LinkedListNode* toDel = temp->mNext;

            
                temp->setNext(temp->mNext->mNext);
                    std::cout<<"OK2 here"<<std::endl;

                if (temp->mNext)
                {
                    temp->mNext->setPrev(temp);
                }
                createNewActionGroup();
                addEdge(temp, temp->mNext, true);
                while(temp->mNext)
                {
                    temp->mNext->mTargetPosition += sf::Vector2f(-100, 0);
                    moveNode(temp->mNext, temp->mNext->mTargetPosition, mAnimationSpeed, true);
                    temp = temp->mNext;
                }

                createNewActionGroup();
                deleteNode(toDel);
                
                break;

            }
            else temp = temp->mNext;
        } 
    }
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

    curNode->setNote(std::to_string(curNode->mIndex));

    moveNode(curNode, curPos, 0.5f, false);
    
    int rowIndex = curNode->mIndex / maxRowNode;
    sf::Vector2f nextPos;

    if ((curNode->mIndex + 1) % maxRowNode == 0) // last node in a row
    {
        nextPos = curPos + sf::Vector2f(0, 100);
    }
    else
    {
        if (rowIndex % 2 == 0) // even row
            nextPos = curPos + sf::Vector2f(100, 0);
        else // odd row
            nextPos = curPos + sf::Vector2f(-100, 0);
    }

    // sf::Vector2f nextPos = curPos + sf::Vector2f(100, 0);
    align(curNode->mNext, nextPos);
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