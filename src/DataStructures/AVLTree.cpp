#include <DataStructures/AVLTree.hpp>
#include <queue>
#include <iostream>

void AVLTree::align(TreeNode* node)
{
    if (!node) 
    {
        return;
    }

    if (node->getParent()) // Not root node
    {
        // std::cerr << node->getValue() << " ";
        if (node->isLeftChild()) // is left child
        {
            // node->moveTo(node->getParent()->getPosition() + sf::Vector2f(-500 / (1 << node->getLevel()), 100));
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

AVLTree::AVLTree():
    mRoot(nullptr)
{
    
}

// void AVLTree::remove(int value)
// {
//     mRoot = remove(mRoot, value);
// }

bool AVLTree::search(int value)
{
    // mRoot->highlight(sf::Color::Yellow, 0.f, 1.f);
    return mRoot->search(mRoot, value);
}

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->setParent(prev);
        return node;
    }
    if (value < node->getValue())
        node->setLeft(insert(node->getLeft(), node, value));
    
    if (value > node->getValue())
        node->setRight(insert(node->getRight(), node, value));

    node = node->update_Height(node);
    node = node->balance(node);
    return node;
}


void AVLTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    if (mRoot) mRoot->draw(target, states);
}

void AVLTree::update(sf::Time dt)   
{
    if (mRoot) mRoot->update(dt);
}

void AVLTree::updateLevel(TreeNode* node, int level)
{
    if (!node) return;
    node->setLevel(level);
    updateLevel(node->getLeft(), level + 1);
    updateLevel(node->getRight(), level + 1);
}

void AVLTree::updateHeight(TreeNode* node, int level)
{

}

void AVLTree::leftRotate()
{
    if (!mRoot || !mRoot->getRight()) return;
    // lR(mRoot);
    mRoot = mRoot->leftRotate(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}

void AVLTree::rightRotate()
{
    if (!mRoot || !mRoot->getLeft()) return;
    rR(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}

void AVLTree::lR(TreeNode* &root)
{
    if (!root || !root->getRight()) return;
    TreeNode* newRoot = root->getRight();
    if (newRoot->getLeft()) newRoot->getLeft()->setParent(root);
    root->setRight(newRoot->getLeft());
    newRoot->setParent(nullptr);
    root->setParent(newRoot);
    newRoot->setLeft(root);
    root = newRoot;
    std::cerr << root->getValue() << "\n";
}

void AVLTree::rR(TreeNode* &root)
{
    if (!root || !root->getLeft()) return;
    TreeNode* newRoot = root->getLeft();
    newRoot->setRight(root);
    root = newRoot;
}