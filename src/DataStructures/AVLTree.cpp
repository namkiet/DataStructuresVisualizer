#include <DataStructures/AVLTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

void AVLTree::align(TreeNode* node)
{
    return ;


    // if (!node) 
    // {
    //     return;
    // }

    // if (node->mParent) // Not root node
    // {
    //     // std::cerr << node->mValue << " ";
    //     if (node->isLeftChild()) // is left child
    //     {
    //         // node->moveTo(node->mParent->getPosition() + sf::Vector2f(-500 / (1 << node->mLevel), 100));
    //         node->moveTo(sf::Vector2f(-500 / (1 << node->mLevel), 100));
            
    //         // node->setPosition(sf::Vector2f(20 ,0));
    //     }
    //     else if (node->isRightChild()) // is right child
    //     {
    //         // node->moveTo(node->mParent->getPosition() + sf::Vector2f(500 / (1 << node->mLevel), 100));
    //         // node->moveTo(sf::Vector2f(500 / (1 << node->mLevel), 100));
    //         // node->setPosition(sf::Vector2f(100, 0));
    //     }
    // }
    // else
    // {
    //     node->moveTo(sf::Vector2f(0, 0));
    //     std::cerr << "HELLO";
    // }

    // align(node->mLeft);
    // align(node->mRight);
}

void AVLTree::insert(int value)
{
    mRoot = insert(mRoot, nullptr, value);
    updateLevel(mRoot, 0);
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
    return search(mRoot, value);
}

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;

        // mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, sf::Vector2f(-100, 100), 1.0f));

        if (prev) // Not root node
        {
            node->mLevel = node->mParent->mLevel + 1;
            if (value < prev->mValue) // is left child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, sf::Vector2f(-500 / (1 << node->mLevel), 100), 0.5f));
            else // is right child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, sf::Vector2f(500 / (1 << node->mLevel), 100), 0.5f));
        }
        else
        {
            node->mLevel = 0;
        }

        return node;
    }


    // mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Red, 1.0f));

    if (value < node->mValue)
        node->mLeft = insert(node->mLeft, node, value);
    
    if (value > node->mValue)
        node->mRight = insert(node->mRight, node, value);

    // node = node->update_Height(node);
    // node = node->balance(node);
    return node;
}

bool AVLTree::search(TreeNode* node, int value)
{
    if (!node) return false;
    if (value == node->mValue)
    {
        mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Blue, 0.3f));
        mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Blue, 0.3f));
        mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Blue, 0.3f));
        return true;
    }
    mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Red, 1.0f));
    if (value < node->mValue) return search(node->mLeft, value);
    if (value > node->mValue) return search(node->mRight, value);

    return true;
}


void AVLTree::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    if (mRoot) mRoot->draw(target, states);
}

void AVLTree::update(sf::Time dt)   
{
    mAnimationQueue.update(dt);
    if (mRoot) mRoot->update(dt);
}

void AVLTree::updateLevel(TreeNode* node, int level)
{
    if (!node) return;
    node->mLevel = level ;
    updateLevel(node->mLeft, level + 1);
    updateLevel(node->mRight, level + 1);
}

void AVLTree::updateHeight(TreeNode* node, int level)
{

}

void AVLTree::leftRotate()
{
    if (!mRoot || !mRoot->mRight) return;
    lR(mRoot);
    // mRoot = mRoot->leftRotate(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}

void AVLTree::rightRotate()
{
    if (!mRoot || !mRoot->mLeft) return;
    rR(mRoot);
    updateLevel(mRoot, 0);
    align(mRoot);
}

void AVLTree::lR(TreeNode* &root)
{
    if (!root || !root->mRight) return;
    TreeNode* newRoot = root->mRight;
    if (newRoot->mLeft) newRoot->mLeft->mParent = root;
    root->mRight = newRoot->mLeft;
    newRoot->mParent = nullptr;
    root->mParent = newRoot;
    newRoot->mLeft = root;
    root = newRoot;
    std::cerr << root->mValue << "\n";
}

void AVLTree::rR(TreeNode* &root)
{
    if (!root || !root->mLeft) return;
    TreeNode* newRoot = root->mLeft;
    newRoot->mRight = root;
    root = newRoot;
}