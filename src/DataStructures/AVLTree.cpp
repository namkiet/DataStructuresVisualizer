#include <DataStructures/AVLTree.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree():
    mRoot(nullptr)
{
    
}

void AVLTree::align(AVLNode* node)
{
    if (!node) 
    {
        return;
    }

    // node = dynamic_cast<AVLNode*>(node);

    if (node->getParent()) // Not root node
    {
        // std::cerr << node->getValue() << " ";
        if (node->isLeftChild()) // is left child
        {
            // node->moveTo(node->getPareznt()->getPosition() + sf::Vector2f(-500 / (1 << node->getLevel()), 100));
            node->moveTo(sf::Vector2f(-500 / (1 << node->getLevel()), 100));
            
            // node->setPosition(sf::Vector2f(20 ,0));
        }
        else if (node->isRightChild()) // is right child
        {
            // node->moveTo(node->getParent()->getPosition() + sf::Vector2f(500 / (1 << node->getLevel()), 100));
            node->moveTo(sf::Vector2f(500 / (1 << node->getLevel()), 100));
            // node->setPosition(sf::Vector2f(100, 0));
        }
    }
    else
    {
        node->moveTo(sf::Vector2f(0, 0));
        std::cerr << "HELLO";
    }

    align(node->getLeft());
    align(node->getRight());
}

void AVLTree::insert(int value)
{
    mRoot = mRoot->insert(mRoot, nullptr, value);
    updateLevel(mRoot, 0);
    align(mRoot);
}

bool AVLTree::search(int value)
{
    return mRoot->search(mRoot, value);
}


void AVLTree::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    if (mRoot)
        target.draw(*mRoot, states);
}

void AVLTree::updateCurrent(sf::Time dt)   
{
    if (mRoot) mRoot->update(dt);
}

void AVLTree::updateLevel(AVLNode* node, int level)
{
    if (!node) return;
    node->setLevel(level);
    updateLevel(node->getLeft(), level + 1);
    updateLevel(node->getRight(), level + 1);
}

void AVLTree::leftRotate()
{
    if (!mRoot || !mRoot->getRight()) return;
    mRoot = mRoot->leftRotate(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}

void AVLTree::rightRotate()
{
    if (!mRoot || !mRoot->getLeft()) return;
    mRoot = mRoot->rightRotate(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}