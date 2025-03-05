#include <DataStructures/AVLNode.hpp>
#include <Core/Utility.hpp>
#include <memory>
#include <iostream>

AVLNode::AVLNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    TreeNode(value, radius, fillColor, outlineColor),
    mLeft(nullptr),
    mRight(nullptr),
    mParent(nullptr)
{
}


void AVLNode::update(sf::Time dt)
{
    updateCurrent(dt);
    if (mLeft) mLeft->updateCurrent(dt);
    if (mRight) mRight->updateCurrent(dt);
}

void AVLNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    if (mLeft) mLeft->drawCurrent(target, states);
    if (mRight) mRight->drawCurrent(target, states);
}

AVLNode* AVLNode::getLeft()
{
    return mLeft;
}

AVLNode* AVLNode::getRight()
{
    return mRight;
}

AVLNode* AVLNode::getParent()
{
    return mParent;
}

void AVLNode::setLeft(AVLNode* node)
{    
    // if (!mLeft) {
    //     mLeft = node;
    //     node->mParent = this;
    //     // attachChild(node);
    //     return;
    // }
    // if (mLeft->mValue == node->mValue) return;

    // // mLeft = node;
    // if (node->mParent)
    mLeft = node;
}

void AVLNode::setRight(AVLNode* node)
{    
    mRight = node;
}

void AVLNode::setParent(AVLNode* node)
{
    mParent = node;
}

int AVLNode::getHeight()
{
    return mHeight;
}

int AVLNode::getLevel()
{
    return mLevel;
}

void AVLNode::setHeight(int height)
{
    mHeight = height;
}

void AVLNode::setLevel(int level)
{
    mLevel = level;
}

void AVLNode::updateHeight()
{
    
}

void AVLNode::updateLevel()
{
}

void AVLNode::updateLevel(AVLNode* &node, int level)
{
}

bool AVLNode::isLeftChild()
{
    if (!mParent || !mParent->mLeft) return false;
    return mValue == mParent->mLeft->mValue;
}

bool AVLNode::isRightChild()
{
    if (!mParent || !mParent->mRight) return false;
    return mValue == mParent->mRight->mValue;
}

int AVLNode::get_Height(AVLNode* root) {
    return root ? root->getHeight() : 0;
}

AVLNode* AVLNode::update_Height(AVLNode* root) {
    if (!root) return nullptr;
    root->setHeight(1 + std::max(get_Height(root->mLeft), get_Height(root->mRight)));
    return root;
}

int AVLNode::get_bf(AVLNode* root) {
    return root ? get_Height(root->mLeft) - get_Height(root->mRight) : 0;
}

AVLNode* AVLNode::leftRotate(AVLNode* x) {
    if (!x || !x->mRight) return x;
    AVLNode* y = x->mRight;
    AVLNode* T2 = y->mLeft;

    y->mLeft = x;
    x->mRight = T2;    
    y->mParent = x->mParent;
    x->mParent = y;
    if (T2) T2->mParent = x;
    x = update_Height(x);
    y = update_Height(y);
    return y;
}

AVLNode* AVLNode::rightRotate(AVLNode* y) {
    if (!y || !y->mLeft) return y;
    AVLNode* x = y->mLeft;
    AVLNode* T2 = x->mRight;

    x->mRight = y;
    y->mLeft = T2;    
    x->mParent = y->mParent;
    y->mParent = x;
    if (T2) T2->mParent = y;
    y = update_Height(y);
    x = update_Height(x);
    return x;
}

AVLNode* AVLNode::balance(AVLNode* root) {
    if (!root) return nullptr;
    int bf = get_bf(root);
    if (bf > 1) {
        if (get_bf(root->mLeft) >= 0)
            return rightRotate(root);
        else {
            root->mLeft = leftRotate(root->mLeft);
            return rightRotate(root);
        }
    }
    if (bf < -1) {
        if (get_bf(root->mRight) <= 0)
            return leftRotate(root);
        else {
            root->mRight = rightRotate(root->mRight);
            return leftRotate(root);
        }
    }
    
    return root;
}

AVLNode* AVLNode::insert(AVLNode* node, AVLNode* prev, int value)
{
    if (!node)
    {
        node = new AVLNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;
        return node;
    }

    // node->highlight(sf::Color::Yellow, 0.f, 1.f);
    if (value < node->mValue) node->mLeft = insert(node->mLeft, node, value);
    if (value > node->mValue) node->mRight = insert(node->mRight, node, value);

    node = update_Height(node);
    node = balance(node);
    return node;
}



bool AVLNode::search(AVLNode* node, int value)
{
    if (!node) return false;

    // if (node->mParent)
    //     node->highlight(sf::Color::Yellow, node->mParent->mDuration - 2.f, 1.f);
    // else
        node->highlight(sf::Color::Yellow, 0.f, 1.f);

    if (value < node->mValue) return search(node->mLeft, value);
    if (value > node->mValue) return search(node->mRight, value);
    return true;
}