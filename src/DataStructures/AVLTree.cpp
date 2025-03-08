#include <DataStructures/AVLTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree():
    mRoot(nullptr)
{    
}

void AVLTree::insert(int value)
{
    mRoot = insert(mRoot, nullptr, value);
    // updateLevel(mRoot, 0);
}

// void AVLTree::remove(int value)
// {
//     mRoot = remove(mRoot, value);
// }

bool AVLTree::search(int value)
{
    return search(mRoot, value);
}

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (node)
        mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Red, 0.75f));

    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;
        if (prev) // Not root node
        {
            node->setOpacity(0);
            node->mLevel = node->mParent->mLevel + 1;
            if (value < prev->mValue) // is left child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, sf::Vector2f(-500 / (1 << node->mLevel), 100), 0.75f, true));
            else // is right child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, sf::Vector2f(500 / (1 << node->mLevel), 100), 0.75f, true));
        }
        else
            node->mLevel = 0;
        return node;
    }
    if (value < node->mValue) node->mLeft = insert(node->mLeft, node, value);
    if (value > node->mValue) node->mRight = insert(node->mRight, node, value);

    // node = updateHeight(node);
    // node = balance(node);
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

int AVLTree::getHeight(TreeNode* root) {
    return root ? root->mHeight : 0;
}

int AVLTree::getBalanceFactor(TreeNode* root) 
{
    return root ? getHeight(root->mLeft) - getHeight(root->mRight) : 0;
}

TreeNode* AVLTree::updateHeight(TreeNode* root)
{
    if (!root) return nullptr;
    root->mHeight = 1 + fmax(getHeight(root->mLeft), getHeight(root->mRight));
    return root;
}

TreeNode* AVLTree::leftRotate(TreeNode* root)
{
    if (!root || !root->mRight) return root;
    std::cerr <<  "LEFT\n";
    TreeNode* newRoot = root->mRight;

    root->mRight = newRoot->mLeft;
    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    newRoot->mParent = root->mParent;
    if  (root->mParent)
    {
        if (root->mParent->mLeft == root)
            root->mParent->mLeft = newRoot;
        else
            root->mParent->mRight = newRoot;
    }

    root->mParent = newRoot;
    newRoot->mLeft = root;

    mAnimationQueue.addAnimation(std::make_unique<NodeMove>(root, sf::Vector2f(0 , 0), 0.75f, false));

    return newRoot;
}

TreeNode* AVLTree::rightRotate(TreeNode* root)
{
    if (!root || !root->mLeft) return root;
    std::cerr <<  "RIGHT\n";
    TreeNode* newRoot = root->mLeft;

    root->mLeft = newRoot->mRight;
    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    newRoot->mParent = root->mParent;
    if  (root->mParent)
    {
        if (root->mParent->mLeft == root)
            root->mParent->mLeft = newRoot;
        else
            root->mParent->mRight = newRoot;
    }

    root->mParent = newRoot;
    newRoot->mRight = root;

    return newRoot;
}

TreeNode* AVLTree::balance(TreeNode* root)
{
    if (!root) return nullptr;
    int bf = getBalanceFactor(root);
    if (bf > 1) {
        if (getBalanceFactor(root->mLeft) >= 0)
            return rightRotate(root);
        else {
            root->mLeft = leftRotate(root->mLeft);
            return rightRotate(root);
        }
    }
    if (bf < -1) {
        if (getBalanceFactor(root->mRight) <= 0)
            return leftRotate(root);
        else {
            root->mRight = rightRotate(root->mRight);
            return leftRotate(root);
        }
    }   
    return root;
}

void AVLTree::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mRoot) mRoot->draw(target, states);
}

void AVLTree::updateCurrent(sf::Time dt)   
{
    mAnimationQueue.update(dt);
    if (mRoot) mRoot->update(dt);
}


void AVLTree::leftRotate()
{
    mRoot = leftRotate(mRoot);
    std::cerr << mRoot->mValue << "\n";
}