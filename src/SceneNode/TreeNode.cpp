#include <SceneNode/TreeNode.hpp>
#include <Core/Utility.hpp>
#include <memory>
#include <iostream>

TreeNode::TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor):
    mIsMoving(false),
    mValue(value),
    mLeft(nullptr),
    mRight(nullptr),
    mParent(nullptr)
{  
    mShape.setRadius(radius);
    mShape.setOrigin(sf::Vector2f(radius, radius));
    mShape.setFillColor(fillColor);
    mShape.setOutlineColor(outlineColor);
    mShape.setOutlineThickness(2);
    mShape.setPointCount(10000);

    font.loadFromFile("assets/fonts/jetbrains.ttf");
    mText.setColor(sf::Color::Black);
    mText.setCharacterSize(radius * 0.75);
    mText.setFont(font);
    mText.setString(std::to_string(value));
    centerOrigin(mText);
    mText.setPosition(mShape.getPosition());
}

void TreeNode::update(sf::Time dt)
{
    updateCurrent(dt);
    if (mIsAnimationFinished)
    {
        if (mLeft) mLeft->update(dt);
        if (mRight) mRight->update(dt);
    }
}


void TreeNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    if (mLeft) mLeft->draw(target, states);
    if (mRight) mRight->draw(target, states);    
}

void TreeNode::updateCurrent(sf::Time dt)
{
    mIsAnimationFinished = true;
    if (mIsMoving)
    {
        mIsAnimationFinished = false;
        sf::Vector2f curPosition = getPosition();
        if (dist(curPosition, mTargetPosition) < 1.f)
        {
            setPosition(mTargetPosition);
            mIsMoving = false;
        }
        else
        {
            sf::Vector2f dir = mTargetPosition - curPosition;
            sf::Vector2f unitDir = dir * (1 / dist(curPosition, mTargetPosition));
            move(unitDir * 400.f * dt.asSeconds());
        }
    }

    if (mIsHighlighted)
    {
        mIsAnimationFinished = false;
        if (mElapsedTime < mDuration)
        {
            if (mElapsedTime >= 0) mShape.setFillColor(sf::Color::Yellow);
            mElapsedTime += dt.asSeconds();
        }
        else
        {
            mShape.setFillColor(sf::Color::White);
            mElapsedTime = 0;
            mDuration = 0;
            mIsHighlighted = 0;
        }
        
    }
}

void TreeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape, states);
    target.draw(mText, states);
}

void TreeNode::moveTo(sf::Vector2f target)
{
    mIsMoving = true;
    mTargetPosition = target;
}

void TreeNode::highlight(sf::Color targetColor, float elapsedTime, float duration)
{
    mIsHighlighted = true;
    mTargetColor = targetColor;
    mElapsedTime = 0.f;
    mDuration = 0.2f;
}

int TreeNode::getValue()
{
    return mValue;
}

TreeNode* TreeNode::getLeft()
{
    return mLeft;
}


TreeNode* TreeNode::getRight()
{
    return mRight;
}


TreeNode* TreeNode::getParent()
{
    return mParent;
}


void TreeNode::setLeft(TreeNode* node)
{    
    mLeft = node;
}

void TreeNode::setRight(TreeNode* node)
{    
    mRight = node;
}

void TreeNode::setParent(TreeNode* node)
{
    mParent = node;
}

int TreeNode::getHeight()
{
    return mHeight;
}

int TreeNode::getLevel()
{
    return mLevel;
}

void TreeNode::setHeight(int height)
{
    mHeight = height;
}

void TreeNode::setLevel(int level)
{
    mLevel = level;
}

void TreeNode::updateHeight()
{
    
}

void TreeNode::updateLevel()
{
}

void TreeNode::updateLevel(TreeNode* &node, int level)
{
}

bool TreeNode::isLeftChild()
{
    if (!mParent || !mParent->mLeft) return false;
    return mValue == mParent->mLeft->getValue();
}

bool TreeNode::isRightChild()
{
    if (!mParent || !mParent->mRight) return false;
    return mValue == mParent->mRight->getValue();
}

int TreeNode::get_Height(TreeNode* root) {
    return root ? root->getHeight() : 0;
}

TreeNode* TreeNode::update_Height(TreeNode* root) {
    if (!root) return nullptr;
    root->setHeight(1 + std::max(get_Height(root->mLeft), get_Height(root->mRight)));
    return root;
}

int TreeNode::get_bf(TreeNode* root) {
    return root ? get_Height(root->mLeft) - get_Height(root->mRight) : 0;
}

TreeNode* TreeNode::leftRotate(TreeNode* x) {
    if (!x || !x->mRight) return x;
    TreeNode* y = x->mRight;
    TreeNode* T2 = y->mLeft;

    y->mLeft = x;
    x->mRight = T2;    
    y->mParent = x->mParent;
    x->mParent = y;
    if (T2) T2->mParent = x;
    x = update_Height(x);
    y = update_Height(y);
    return y;
}

TreeNode* TreeNode::rightRotate(TreeNode* y) {
    if (!y || !y->mLeft) return y;
    TreeNode* x = y->mLeft;
    TreeNode* T2 = x->mRight;

    x->mRight = y;
    y->mLeft = T2;    
    x->mParent = y->mParent;
    y->mParent = x;
    if (T2) T2->mParent = y;
    y = update_Height(y);
    x = update_Height(x);
    return x;
}

TreeNode* TreeNode::balance(TreeNode* root) {
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

TreeNode* TreeNode::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;
        return node;
    }

    // node->highlight(sf::Color::Yellow, 0.f, 1.f);
    if (value < node->mValue) node->mLeft = insert(node->mLeft, node, value);
    if (value > node->mValue) node->mRight = insert(node->mRight, node, value);
    return node;
}



bool TreeNode::search(TreeNode* node, int value)
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