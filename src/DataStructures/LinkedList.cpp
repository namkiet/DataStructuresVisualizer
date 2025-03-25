#include <DataStructures/LinkedList.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/Edge.hpp>
#include <DataStructures/DS.hpp>
#include <Core/Animation.hpp>


LinkedList::LinkedList(): mHead(nullptr) {}


void LinkedList::InsertAtHead(int value){
    
    LinkedListNode* newHead = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    newHead->setPosition(sf::Vector2f(400, 550));
    newHead->setNext(mHead);
    newHead->setOpacity(1);
    newHead->setPrev(nullptr);

    if(mHead){
        mHead->setPrev(newHead);
    }
    mHead = newHead;
    createNewActionGroup();
    addNode(mHead);
    highlightNode(mHead, sf::Color::Red, mAnimationSpeed);

    createNewActionGroup();
    LinkedListNode* temp = mHead->getNext();
    while(temp != NULL){
        moveNode(temp, temp->getPosition() + sf::Vector2f(100, 0), mAnimationSpeed, true);
        temp = temp->getNext();
    }
    createNewActionGroup();
    moveNode(mHead, sf::Vector2f(450,400), mAnimationSpeed, true);
    addEdge(mHead, mHead->getNext(), true);
    
}

void LinkedList::InsertAtLast(int value){
    if(mHead == nullptr){
        InsertAtHead(value);
        return;
    }

    LinkedListNode* newLast = new LinkedListNode(value, 16.f, sf::Color::White, sf::Color::Black);
    newLast->setPosition(sf::Vector2f(400, 550));
    newLast->setNext(nullptr);

    createNewActionGroup();
    addNode(newLast);
    highlightNode(newLast, sf::Color::Red, mAnimationSpeed);

    LinkedListNode* temp1 = mHead;
    while(temp1 -> getNext()){
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

    moveNode(newLast, temp1->getPosition() + sf::Vector2f(100.f,0.f), mAnimationSpeed, true);
}

void LinkedList::insert(int value){
    std::cout<<"insert nothing here"<<std::endl;
}
bool LinkedList::search(int value){
    if(!mHead){
        std::cout<<"Not found "<<value<<std::endl;
        return true;
    }
    LinkedListNode* temp = mHead;
    while(temp){
        createNewActionGroup();
        highlightNode(temp, sf::Color::Red, mAnimationSpeed);
        if (temp->mValue == value){
            createNewActionGroup();
            highlightNode(temp, sf::Color::Green, mAnimationSpeed);
            createNewActionGroup();
            highlightNode(temp, sf::Color::Green, mAnimationSpeed);
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
void LinkedList::remove(int value){
    if(!mHead){
        std::cout<<"Not found "<<value<<std::endl;
    }
    LinkedListNode* temp = mHead;
    if(temp->mValue == value){

    }
}