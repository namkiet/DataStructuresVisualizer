#include <DataStructures/AVLTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree(): mRoot(nullptr) {}

void AVLTree::insert(int value)
{
    if (updateStepCallback) updateStepCallback(0);
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

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 16.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;
        addNode(node);
        addEdge(node, nullptr);
        addEdge(node, nullptr);
        if (prev) // Not root node
        {
            node->setPosition(prev->getPosition());
            node->setOpacity(0);
            node->mLevel = prev->mLevel + 1;
            if (value < prev->mValue) // is left child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, prev->getPosition() + sf::Vector2f(-mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), 0.25f, true));
            else // is right child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, prev->getPosition() + sf::Vector2f(mMaxWidth / (1 << (node->mLevel + 1)), mVerticalSpacing), 0.25f, true));
            
            // findEdge(prev, nullptr)->mTo = node;
            mAnimationQueue.addAnimation(std::make_unique<EdgeMove>(mEdgeList, prev, nullptr, node, 0.5f));
        }
        else
        {
            node->mLevel = 0;
            node->setPosition(sf::Vector2f(mMaxWidth / 2, mVerticalSpacing));
        }
        return node;
    }

    mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Red, 0.25f));
    if (value < node->mValue) node->mLeft = insert(node->mLeft, node, value);
    if (value > node->mValue) node->mRight = insert(node->mRight, node, value);

    node = updateHeight(node);
    node = balance(node);
    return node;
}

TreeNode* AVLTree::remove(TreeNode* node, int value)
{
    if (!node) return nullptr;

    if (value < node->mValue) node->mLeft = remove(node->mLeft, value);
    else if (value > node->mValue) node->mRight = remove(node->mRight, value);
    else 
    {
        if (!node->mLeft)
        {
            if (node->mParent)
            {
                mAnimationQueue.addAnimation(std::make_unique<EdgeMove>(mEdgeList, node->mParent, node, nullptr, 3.f));
                if (node == node->mParent->mLeft)
                {
                    node->mParent->mLeft = nullptr;
                }
                if (node == node->mParent->mRight)
                {
                    node->mParent->mRight = nullptr;
                }
            }

            TreeNode* tmp = node->mRight;
            removeEdge(node, node->mLeft);
            removeEdge(node, node->mRight);
            // delete node;
            return tmp;
        } 
        else if (!node->mRight)
        {
            if (node->mParent)
            {
                mAnimationQueue.addAnimation(std::make_unique<EdgeMove>(mEdgeList, node->mParent, node, nullptr, 3.f));
                if (node == node->mParent->mLeft)
                {
                    node->mParent->mLeft = nullptr;
                }
                if (node == node->mParent->mRight)
                {
                    node->mParent->mRight = nullptr;
                }
            }

            TreeNode* tmp = node->mLeft;
            removeEdge(node, node->mLeft);
            removeEdge(node, node->mRight);
            // delete node;
            return tmp;
        }
        else
        {
            TreeNode* prev = node;
            TreeNode* cur = node->mRight;

            TreeNode* l = node->mLeft;
            TreeNode* r = node->mRight;
            TreeNode* p = node->mParent;

            while (cur && cur->mLeft) 
            {
                prev = cur;
                cur = cur->mLeft;
            }

            std::vector<std::unique_ptr<Animation>> deleteAnimations;

            mAnimationQueue.addAnimation(std::make_unique<EdgeMove>(mEdgeList, cur, cur->mLeft, node->mLeft, 3.5f));
            mAnimationQueue.addAnimation(std::make_unique<EdgeMove>(mEdgeList, cur, cur->mRight, node->mRight, 3.5f));

            if (cur == prev->mLeft)
            {
                deleteAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, prev, prev->mLeft, nullptr, 0.5f));
                prev->mLeft = nullptr;
            }
            if (cur == prev->mRight)
            {
                deleteAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, prev, prev->mRight, nullptr, 0.5f));
                prev->mRight = nullptr;
            }            

            if (p)
            {
                if (node == p->mLeft)
                {
                    deleteAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, p, p->mLeft, cur , 0.5f));
                    p->mLeft = cur;
                }
                if (node == p->mRight)
                {
                    deleteAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, p, p->mRight, cur, 0.5f));
                    p->mRight = cur;
                }
            }

            cur->mParent = node->mParent;
            cur->mLeft = node->mLeft;
            cur->mRight = node->mRight;

            if (node->mLeft) node->mLeft->mParent = cur;
            if (node->mRight) node->mRight->mParent = cur;

            mAnimationQueue.addAnimationGroup(deleteAnimations);

            removeEdge(node, node->mLeft);
            removeEdge(node, node->mRight);

            return cur;
        }
    }

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
    if (updateStepCallback) updateStepCallback(1);
    if (!root) return nullptr;
    root->mHeight = 1 + std::max(getHeight(root->mLeft), getHeight(root->mRight));
    return root;
}

TreeNode* AVLTree::leftRotate(TreeNode* root)
{
    if (!root || !root->mRight) return root;
    std::vector<std::unique_ptr<Animation>> leftRotateAnimations;
    
    TreeNode* newRoot = root->mRight;
    TreeNode* par = root->mParent;

    newRoot->mParent = par;

    if (par)
    {
        leftRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, par, root, newRoot, 0.5f));
        if (par->mLeft == root) 
            par->mLeft = newRoot;
        else 
            par->mRight = newRoot;
    }

    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    leftRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, root, root->mRight, newRoot->mLeft, 0.5f));
    root->mRight = newRoot->mLeft;

    leftRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, newRoot, newRoot->mLeft, root, 0.5f));
    newRoot->mLeft = root;

    root->mParent = newRoot;

    root = updateHeight(root);
    newRoot = updateHeight(newRoot);

    mAnimationQueue.addAnimationGroup(leftRotateAnimations);

    align(root->mValue == mRoot->mValue ? newRoot : mRoot);
    return newRoot;
}

TreeNode* AVLTree::rightRotate(TreeNode* root)
{
    if (!root || !root->mLeft) return root;
    std::vector<std::unique_ptr<Animation>> rightRotateAnimations;

    TreeNode* newRoot = root->mLeft;
    TreeNode* par = root->mParent;

    newRoot->mParent = par;

    if (par)
    {
        rightRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, par, root, newRoot, 0.5f));
        if (par->mRight == root) 
            par->mRight = newRoot;
        else 
            par->mLeft = newRoot;
    }

    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    rightRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, root, root->mLeft, newRoot->mRight, 0.5f));
    root->mLeft = newRoot->mRight;

    rightRotateAnimations.push_back(std::make_unique<EdgeMove>(mEdgeList, newRoot, newRoot->mRight, root, 0.5f));
    newRoot->mRight = root;
    
    root->mParent = newRoot;

    root = updateHeight(root);
    newRoot = updateHeight(newRoot);

    mAnimationQueue.addAnimationGroup(rightRotateAnimations);

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
            if (updateStepCallback) updateStepCallback(3);
            return rightRotate(root);
        }
        else {
            if (updateStepCallback) updateStepCallback(5);
            root->mLeft = leftRotate(root->mLeft); // LR
            std::cerr << "---------\n";
            return rightRotate(root);
        }
    }
    if (bf < -1) {
        if (getBalanceFactor(root->mRight) <= 0) // RR
        {
            if (updateStepCallback) updateStepCallback(4);
            return leftRotate(root);
        }
        else {
            if (updateStepCallback) updateStepCallback(6);
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

void AVLTree::align(TreeNode* root)
{
    if (!root) return;
    std::vector<std::unique_ptr<Animation>> alignAnimation;
    sf::Vector2f curPos = sf::Vector2f(mMaxWidth / 2, mVerticalSpacing);
    std::queue<std::pair<TreeNode*, sf::Vector2f>> q;
    q.push(std::make_pair(root, sf::Vector2f(0, 0)));
    while (!q.empty())
    {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++)
        {
            TreeNode* cur = q.front().first;
            sf::Vector2f prevPos = q.front().second;
            q.pop();

            sf::Vector2f curPos;
            if (cur->mParent)
            {
                cur->mLevel = cur->mParent->mLevel + 1;
                if (cur->mValue < cur->mParent->mValue) // if current node is left child
                    curPos = prevPos + sf::Vector2f(-mMaxWidth / (1 << (cur->mLevel + 1)), mVerticalSpacing);
                else // if current node is right child
                    curPos = prevPos + sf::Vector2f(mMaxWidth / (1 << (cur->mLevel + 1)), mVerticalSpacing);
            }
            else
            {
                cur->mLevel = 0;
                curPos = sf::Vector2f(mMaxWidth / 2, mVerticalSpacing);
            }
            alignAnimation.push_back(std::make_unique<NodeMove>(cur, curPos, 0.5f));

            if (cur->mLeft) 
                q.push(std::make_pair(cur->mLeft, curPos));
            
            if (cur->mRight) 
                q.push(std::make_pair(cur->mRight, curPos));
        }
    }
    mAnimationQueue.addAnimationGroup(alignAnimation);
}