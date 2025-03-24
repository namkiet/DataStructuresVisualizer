#include <DataStructures/AVLTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree(): mRoot(nullptr) {}

void AVLTree::insert(int value)
{
    if (updateStepCallback) 
    {
        mActionQueue.pushAction([this](sf::Time dt) {
            this->updateStepCallback(0);
            return true;
        });
    }

    createNewActionGroup();

    mActionQueue.pushAction([=](sf::Time){
        mRoot = insert(mRoot, nullptr, value);
        align(mRoot);
        return true;
    });
}

void AVLTree::remove(int value)
{
    mRoot = remove(mRoot, value);
    align(mRoot);

    mActionQueue.pushAction([this](sf::Time dt) mutable->bool {
        for (auto& e: mEdgeList)
        {
            if (e)
            std::cerr << e->mFrom->mValue << " - " << (e->mTo ? e->mTo->mValue : -1) << "\n";
            else  
            std::cerr << "Invalid Edge here \n";
        }
        std::cerr << "\n";

        std::queue<TreeNode*> q;
        q.push(mRoot);

        while (!q.empty())
        {
            TreeNode* cur = q.front();
            q.pop();

            if (!cur) continue; 
            std::cerr << cur->mValue << ": " << (cur->mParent ? cur->mParent->mValue : -1) << " " << (cur->mLeft ? cur->mLeft->mValue : -1) << " " << (cur->mRight ? cur->mRight->mValue : -1) << "\n";
            q.push(cur->mLeft);
            q.push(cur->mRight);
        }
        return true;
    });
}

bool AVLTree::search(int value)
{
    return search(mRoot, value);
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
                moveNode(node, prev->getPosition() + sf::Vector2f(-mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), mAnimationSpeed, true);
            else // is right child
                moveNode(node, prev->getPosition() + sf::Vector2f(mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), mAnimationSpeed, true);
            

            createNewActionGroup();
            moveEdge(prev, nullptr, node, mAnimationSpeed);
        }
        else
        {
            node->mLevel = 0;
            node->setPosition(sf::Vector2f(mMaxWidth / 2, mVerticalSpacing));
        }
        return node;
    }
    
    createNewActionGroup();
    highlightNode(node, sf::Color::Red, mAnimationSpeed);

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
            mActionQueue.pushAction(Action::ChangeNodeValue(node, cur->mValue, 0.5f));
            mActionQueue.pushAction(Action::ChangeNodeValue(cur, value, 0.5f));

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
            highlightNode(node, sf::Color::Blue, mAnimationSpeed * 0.4f);
        }
        return true;
    }

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, mAnimationSpeed);
    
    if (value < node->mValue) 
    {
        createNewActionGroup();
        traverseEdge(node, node->mLeft, sf::Color::Red, mAnimationSpeed);
        return search(node->mLeft, value);
    }
    if (value > node->mValue)
    {
        createNewActionGroup();
        traverseEdge(node, node->mRight, sf::Color::Red, mAnimationSpeed);
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
        moveEdge(par, root, newRoot, mAnimationSpeed);
        if (par->mLeft == root) 
            par->mLeft = newRoot;
        else 
            par->mRight = newRoot;
    }

    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    moveEdge(root, root->mRight, newRoot->mLeft, mAnimationSpeed);
    root->mRight = newRoot->mLeft;

    moveEdge(newRoot, newRoot->mLeft, root, mAnimationSpeed);
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
        moveEdge(par, root, newRoot, mAnimationSpeed);
        if (par->mRight == root) 
            par->mRight = newRoot;
        else 
            par->mLeft = newRoot;
    }

    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    moveEdge(root, root->mLeft, newRoot->mRight, mAnimationSpeed);
    root->mLeft = newRoot->mRight;

    moveEdge(newRoot, newRoot->mRight, root, mAnimationSpeed);
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
    if (bf > 1) {
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
        else {
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
    if (bf < -1) {
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
        else {
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

    moveNode(curNode, curPos, mAnimationSpeed, false);
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(curNode->mLeft, leftChildPos, newSpacingX, newSpacingY);
    align(curNode->mRight, rightChildPos, newSpacingX, newSpacingY);
}