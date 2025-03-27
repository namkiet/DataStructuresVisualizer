#include <DataStructures/AVLTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree(): mRoot(nullptr) {}

void AVLTree::insert(int value)
{
    mRoot = insert(mRoot, nullptr, value);
    align(mRoot);
}

void AVLTree::remove(int value)
{
    mRoot = remove(mRoot, value);
    align(mRoot);
}

bool AVLTree::search(int value)
{
    return search(mRoot, value);
}

void AVLTree::empty()
{
    DS::empty();
    mRoot = nullptr;
}

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 16.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;

        createNewActionGroup();
        addNode(node);
        addEdge(node, nullptr);
        addEdge(node, nullptr);

        if (prev) // Not root node
        {
            node->setPosition(prev->getPosition());
            node->setOpacity(0);
            node->mLevel = prev->mLevel + 1;

            createNewActionGroup();
            if (value < prev->mValue) // is left child
                moveNode(node, prev->getPosition() + sf::Vector2f(-mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), 0.5f, true);
            else // is right child
                moveNode(node, prev->getPosition() + sf::Vector2f(mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), 0.5f, true);
            

            createNewActionGroup();
            moveEdge(prev, nullptr, node, 0.5f);
        }
        else
        {
            node->mLevel = 0;
            node->setPosition(sf::Vector2f(mMaxWidth / 2, mVerticalSpacing));
        }
        return node;
    }
    
    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.5f);

    if (value < node->mValue) // Left traveral
    {
        createNewActionGroup();
        traverseEdge(node, node->mLeft, sf::Color::Red, 0.5f);
        node->mLeft = insert(node->mLeft, node, value);
    }

    if (value > node->mValue) // Right traversal
    {
        createNewActionGroup();
        traverseEdge(node, node->mRight, sf::Color::Red, 0.5f);
        node->mRight = insert(node->mRight, node, value);
    }

    node = updateHeight(node);
    node = balance(node);
    return node;
}

TreeNode* AVLTree::remove(TreeNode* node, int value)
{
    if (!node) return nullptr;

    if (value < node->mValue) 
        node->mLeft = remove(node->mLeft, value);
    else if (value > node->mValue) 
        node->mRight = remove(node->mRight, value);
    else 
    {
        if (!node->mLeft || !node->mRight) {
            TreeNode* temp = node->mLeft ? node->mLeft : node->mRight;

            if (temp)
                temp->mParent = node->mParent;


            createNewActionGroup();
            mActionQueue.pushAction(Action::FadeNode(node, 0.5f));

            // All edges lead TO it now lead to NULL
            for (auto& e: mEdgeList)
            {
                if (e->mTo == node)
                {
                    moveEdge(e->mFrom, node, temp, 0.5f);
                    align(mRoot);
                    break;
                }
            }

            createNewActionGroup();
            removeNode(node); // Remove a node and all edges FROM it

            return temp;
        }
        else
        {
            TreeNode* cur = node->mRight;
            while (cur->mLeft)
                cur = cur->mLeft;
            
            // node->setValue(cur->mValue);
            // cur->setValue(value);
            // mActionQueue.pushAction(Action::ChangeNodeValue(node, cur->mValue, 0.5f));
            // mActionQueue.pushAction(Action::ChangeNodeValue(cur, value, 0.5f));
            // mActionQueue.pushAction(Action::SwapNodeValues(node, cur, 0.5f));
            swapTwoNodes(cur, node);
            createNewActionGroup();

            mActionQueue.pushAction([this, node, value](sf::Time dt) mutable -> bool
            {
                node->mRight = remove(node->mRight, value);   
                return true;
            });

            return node;
        }
    }

    node = updateHeight(node);
    node = balance(node);
    return node;
}

bool AVLTree::search(TreeNode* node, int value)
{
    if (!node) return false;
    if (value == node->mValue)
    {
        int blinkTimes = 3;
        while (blinkTimes--)
        {
            createNewActionGroup();
            highlightNode(node, sf::Color::Blue, 0.5f * 0.4f);
        }
        return true;
    }

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.5f);
    
    if (value < node->mValue) 
    {
        createNewActionGroup();
        traverseEdge(node, node->mLeft, sf::Color::Red, 0.5f);
        return search(node->mLeft, value);
    }
    if (value > node->mValue)
    {
        createNewActionGroup();
        traverseEdge(node, node->mRight, sf::Color::Red, 0.5f);
        return search(node->mRight, value);
    }
    return true;
}

int AVLTree::getHeight(TreeNode* root) {
    if (root)
        return root->mHeight;
    return 0;
}

int AVLTree::getBalanceFactor(TreeNode* root) 
{
    if (root)
        return getHeight(root->mLeft) - getHeight(root->mRight);
    return 0;
}

TreeNode* AVLTree::updateHeight(TreeNode* root)
{
    if (updateStepCallback) updateStepCallback(1);

    if (!root) 
        return nullptr;

    root->mHeight = 1 + std::max(getHeight(root->mLeft), getHeight(root->mRight));
    return root;
}

TreeNode* AVLTree::leftRotate(TreeNode* root)
{
    if (!root || !root->mRight) return root;
    createNewActionGroup();
    
    TreeNode* newRoot = root->mRight;
    TreeNode* par = root->mParent;

    newRoot->mParent = par;

    if (par)
    {
        moveEdge(par, root, newRoot, 0.5f);
        if (par->mLeft == root) 
            par->mLeft = newRoot;
        else 
            par->mRight = newRoot;
    }

    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    moveEdge(root, root->mRight, newRoot->mLeft, 0.5f);
    root->mRight = newRoot->mLeft;

    moveEdge(newRoot, newRoot->mLeft, root, 0.5f);
    newRoot->mLeft = root;

    root->mParent = newRoot;

    root = updateHeight(root);
    newRoot = updateHeight(newRoot);

    align(root->mValue == mRoot->mValue ? newRoot : mRoot);
    return newRoot;
}

TreeNode* AVLTree::rightRotate(TreeNode* root)
{
    if (!root || !root->mLeft) return root;
    createNewActionGroup();

    TreeNode* newRoot = root->mLeft;
    TreeNode* par = root->mParent;

    newRoot->mParent = par;
    if (par)
    {
        moveEdge(par, root, newRoot, 0.5f);
        if (par->mRight == root) 
            par->mRight = newRoot;
        else 
            par->mLeft = newRoot;
    }

    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    moveEdge(root, root->mLeft, newRoot->mRight, 0.5f);
    root->mLeft = newRoot->mRight;

    moveEdge(newRoot, newRoot->mRight, root, 0.5f);
    newRoot->mRight = root;
    
    root->mParent = newRoot;

    root = updateHeight(root);
    newRoot = updateHeight(newRoot);

    align(root->mValue == mRoot->mValue ? newRoot : mRoot);
    return newRoot;
}

TreeNode* AVLTree::balance(TreeNode* root)
{
    if (!root) return nullptr;
    int bf = getBalanceFactor(root);


    mActionQueue.pushInstantAction([=](){
        root->setNote("bf = " + std::to_string(bf));
    });

    highlightNode(root, sf::Color::Blue, 1.f);

    mActionQueue.pushInstantAction([=](){
        root->setNote("");
    });

    if (bf > 1) 
    {
        if (getBalanceFactor(root->mLeft) >= 0) // LL
        {
            if (updateStepCallback) 
            {
                mActionQueue.pushAction([this](sf::Time dt) {
                    this->updateStepCallback(3);
                    return true;
                });
            }
            return rightRotate(root);
        }
        else 
        {
            if (updateStepCallback) 
            {
                mActionQueue.pushAction([this](sf::Time dt) {
                    this->updateStepCallback(5);
                    return true;
                });
            }
            root->mLeft = leftRotate(root->mLeft); // LR
            return rightRotate(root);
        }
    }
    if (bf < -1) 
    {
        if (getBalanceFactor(root->mRight) <= 0) // RR
        {
            if (updateStepCallback) 
            {
                mActionQueue.pushAction([this](sf::Time dt) {
                    this->updateStepCallback(4);
                    return true;
                });
            }
            return leftRotate(root);
        }
        else 
        {
            if (updateStepCallback) 
            {
                mActionQueue.pushAction([this](sf::Time dt) {
                    this->updateStepCallback(6);
                    return true;
                });
            }
            root->mRight = rightRotate(root->mRight); // RL
            return leftRotate(root);
        }
    }   
    return root;
}

void AVLTree::leftRotate()
{
    mRoot = leftRotate(mRoot);
    align(mRoot);
}

void AVLTree::rightRotate()
{
    mRoot = rightRotate(mRoot);
    align(mRoot);
}

void AVLTree::align(TreeNode* curNode, sf::Vector2f curPos, float curSpacingX, float curSpacingY) // DFS
{
    if (!curNode) return;

    if (!curNode->mParent)
    {
        curNode->mLevel = 0;
        createNewActionGroup();
    }
    else
        curNode->mLevel = curNode->mParent->mLevel + 1;
    
    std::cerr << curPos.x << "-" << curPos.y << "\n";

    moveNode(curNode, curPos, 0.5f, false);
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(curNode->mLeft, leftChildPos, newSpacingX, newSpacingY);
    align(curNode->mRight, rightChildPos, newSpacingX, newSpacingY);
}

void AVLTree::saveState()
{
    mHistory.push(History(std::move(mNodeList), std::move(mEdgeList), mRoot));
}

void AVLTree::loadState()
{
    if (mHistory.empty())
        return;

    History history = mHistory.top();
    mHistory.pop();

    mNodeList = std::move(history.nodeList);
    mEdgeList = std::move(history.edgeList);
    mRoot = static_cast<TreeNode*>(history.baseNode);
}