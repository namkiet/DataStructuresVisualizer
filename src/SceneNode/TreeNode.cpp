#include <SceneNode/TreeNode.hpp>
#include <Core/Utility.hpp>
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

void TreeNode::updateCurrent(sf::Time dt)
{
    std::cerr << mValue << "\n";
    if (mIsMoving)
    {
        std::cerr << mValue << "\n";
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
            move(unitDir * 200.f * dt.asSeconds());
        }
    }
}

void TreeNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mShape, states);
    target.draw(mText, states);
}

unsigned int TreeNode::getCategory() const
{
    return Category::TreeNode;
}

void TreeNode::moveTo(sf::Vector2f target)
{
    mIsMoving = true;
    mTargetPosition = target;
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

void TreeNode::attachLeft(TreeNode* child) {
    child->moveTo(sf::Vector2f(-100, 100));
    mLeft = child;
    this->SceneNode::attachChild(std::unique_ptr<SceneNode>(child));
}

void TreeNode::attachRight(TreeNode* child) {
    child->moveTo(sf::Vector2f(100, 100));
    mRight = child;
    this->SceneNode::attachChild(std::unique_ptr<SceneNode>(child));
}

int TreeNode::getHeight()
{
    return mHeight;
}

int TreeNode::getLevel()
{
    return mLevel;
}

void TreeNode::updateHeight()
{
    
}

void TreeNode::updateLevel()
{
    
}

std::vector<TreeNode*> TreeNode::getInorderTraversal(TreeNode* node)
{
    std::vector<TreeNode*> nodeList;
    inorder(node, nodeList);
    return nodeList;
}

void TreeNode::inorder(TreeNode* node, std::vector<TreeNode*> &nodeList)
{
    if (!node) return;
    inorder(node->mLeft, nodeList);
    nodeList.push_back(node);
    inorder(node->mRight, nodeList);
}