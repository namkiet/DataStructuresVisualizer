#include <DataStructures/LinkedList.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>


LinkedList::LinkedList(): mHead(nullptr) {}


void LinkedList::InsertAtHead(int value)
{
    
    LinkedListNode* newHead = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    newHead->mTargetPosition = sf::Vector2f(400, 550);
    newHead->setPosition(newHead->mTargetPosition);
    newHead->setNext(mHead);
    newHead->setOpacity(1);
    newHead->setPrev(nullptr);

    if (mHead)
    {
        mHead->setPrev(newHead);
    }
    mHead = newHead;
    createNewActionGroup();
    addNode(mHead);
    highlightNode(mHead, sf::Color::Red, mAnimationSpeed);

    createNewActionGroup();
    LinkedListNode* temp = mHead->getNext();
    while(temp != NULL)
    {
        temp->mTargetPosition += sf::Vector2f(100, 0);
        moveNode(temp, temp->mTargetPosition, mAnimationSpeed, true);
        temp = temp->getNext();
    }
    createNewActionGroup();
    moveNode(mHead, sf::Vector2f(450,400), mAnimationSpeed, true);
    addEdge(mHead, mHead->getNext(), true);
    
}

void LinkedList::InsertAtLast(int value)
{
    if (!mHead)
    {
        InsertAtHead(value);
        return;
    }

    LinkedListNode* newLast = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    newLast->mTargetPosition = sf::Vector2f(400, 550);
    newLast->setPosition(newLast->mTargetPosition);
    newLast->setNext(nullptr);

    createNewActionGroup();
    addNode(newLast);
    highlightNode(newLast, sf::Color::Red, mAnimationSpeed);

    LinkedListNode* temp1 = mHead;
    while(temp1->getNext())
    {
        createNewActionGroup();
        highlightNode(temp1, sf::Color::Red, mAnimationSpeed);
        createNewActionGroup();
        traverseEdge(temp1, temp1->getNext(), sf::Color::Red, mAnimationSpeed);
        temp1 = temp1->getNext();
    }
    createNewActionGroup();
    highlightNode(temp1, sf::Color::Red, mAnimationSpeed);

    temp1->setNext(newLast);
    newLast->setPrev(temp1);

    createNewActionGroup();
    addEdge(temp1,newLast, true);

    newLast->mTargetPosition = temp1->mTargetPosition + sf::Vector2f(100, 0);
    moveNode(newLast, newLast->mTargetPosition, mAnimationSpeed, true);
}

void LinkedList::insert(int value)
{
    // std::cout<<"insert nothing here"<<std::endl;
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
        traverseEdge(temp, temp->getNext(), sf::Color::Red, mAnimationSpeed);
        temp = temp->getNext();
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
        mHead = temp->getNext();
        if (mHead)
        {
            mHead->setPrev(nullptr);
        }
        temp->mTargetPosition += sf::Vector2f(0, 100);
        createNewActionGroup();
        moveNode(temp, temp->mTargetPosition, mAnimationSpeed, true);
        highlightNode(temp, sf::Color::Red, mAnimationSpeed);
        traverseEdge(temp, temp->getNext(), sf::Color::Red, mAnimationSpeed);

        createNewActionGroup();
        removeEdge(temp, temp->getNext());

        createNewActionGroup();
        deleteNodeEffect(temp, mAnimationSpeed);
        
        LinkedListNode* temp1 = mHead;
        while(temp1)
        {
            std::cout<<"Start loop"<<std::endl;
            temp1->mTargetPosition += sf::Vector2f(-100, 0);
            moveNode(temp1, temp1->mTargetPosition, mAnimationSpeed, true);
            temp1 = temp1->getNext();
            std::cout<<"end loop"<<std::endl;
        }

        createNewActionGroup();
        deleteNode(temp);
    }
    else {
        while(temp->getNext())
        {
            if (temp->getNext()->mValue == value)
            {
                    std::cout<<"Not head"<<std::endl;
                createNewActionGroup();
                highlightNode(temp->getNext(), sf::Color::Red, mAnimationSpeed);
                traverseEdge(temp, temp->getNext(), sf::Color::Red, mAnimationSpeed);
                traverseEdge(temp->getNext(), temp->getNext()->getNext(), sf::Color::Red, mAnimationSpeed);
                    std::cout<<"OK here"<<std::endl;
                createNewActionGroup();
                removeEdge(temp, temp->getNext());
                removeEdge(temp->getNext(), temp->getNext()->getNext());
                createNewActionGroup();
                temp->getNext()->mTargetPosition += temp->getNext()->mTargetPosition + sf::Vector2f(0.f, 100.f);
                moveNode(temp->getNext(), temp->getNext()->mTargetPosition, mAnimationSpeed, true);
                    std::cout<<"OK1 here"<<std::endl;
                createNewActionGroup();
                deleteNodeEffect(temp->getNext(), mAnimationSpeed);

                LinkedListNode* toDel = temp->getNext();

            
                temp->setNext(temp->getNext()->getNext());
                    std::cout<<"OK2 here"<<std::endl;

                if (temp->getNext())
                {
                    temp->getNext()->setPrev(temp);
                }
                createNewActionGroup();
                addEdge(temp, temp->getNext(), true);
                while(temp->getNext())
                {
                    temp->getNext()->mTargetPosition += sf::Vector2f(-100, 0);
                    moveNode(temp->getNext(), temp->getNext()->mTargetPosition, mAnimationSpeed, true);
                    temp = temp->getNext();
                }

                createNewActionGroup();
                deleteNode(toDel);
                
                break;

            }
            else temp = temp->getNext();
        } 
    }
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