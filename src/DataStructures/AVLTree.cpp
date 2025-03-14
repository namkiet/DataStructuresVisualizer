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
    align(mRoot);
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
        mAnimationQueue.addAnimation(std::make_unique<NodeHighlight>(node, sf::Color::Red, 0.25f));

    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->mParent = prev;
        if (prev) // Not root node
        {
            node->setPosition(node->mParent->getPosition());
            node->setOpacity(0);
            node->mLevel = node->mParent->mLevel + 1;
            if (value < prev->mValue) // is left child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, node->mParent->getPosition() + sf::Vector2f(-500 / (1 << node->mLevel), 100), 0.25f, true));
            else // is right child
                mAnimationQueue.addAnimation(std::make_unique<NodeMove>(node, node->mParent->getPosition() + sf::Vector2f(500 / (1 << node->mLevel), 100), 0.25f, true));
            
            addEdge(node->mParent, node);
        }
        else
        {
            node->mLevel = 0;
            node->setPosition(sf::Vector2f(600, 100));
        }
        return node;
    }

    if (value < node->mValue) node->mLeft = insert(node->mLeft, node, value);
    if (value > node->mValue) node->mRight = insert(node->mRight, node, value);

    node = updateHeight(node);
    node = balance(node);
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
    root->mHeight = 1 + std::max(getHeight(root->mLeft), getHeight(root->mRight));
    return root;
}

TreeNode* AVLTree::leftRotate(TreeNode* root)
{
    if (!root || !root->mRight) return root;

    TreeNode* newRoot = root->mRight;

    std::vector<std::unique_ptr<Animation>> animationGroup;
    
    if (root->mParent)
    {
        if (root->mParent->mLeft == root)
        {
            animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root->mParent, root), newRoot));
            root->mParent->mLeft = newRoot;
        }
        else
        {
            animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root->mParent, root), newRoot));
            root->mParent->mRight = newRoot;
        }
    }

    if (root->mRight)
    {
        animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root, root->mRight), newRoot->mLeft));
        root->mRight = newRoot->mLeft;
    }
    else
    {
        addEdge(root, newRoot->mLeft);
        root->mRight = newRoot->mLeft;
    }

    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    newRoot->mParent = root->mParent;

    root->mParent = newRoot;
    root = updateHeight(root);

    if (newRoot->mLeft)
    {
        animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(newRoot, newRoot->mLeft), root));
        newRoot->mLeft = root;
    }
    else
    {
        addEdge(newRoot, root);
        newRoot->mLeft = root;
    }

    newRoot = updateHeight(newRoot);
    mAnimationQueue.addAnimationGroup(animationGroup);

    align(root->mValue == mRoot->mValue ? newRoot : mRoot);
    return newRoot;
}

TreeNode* AVLTree::rightRotate(TreeNode* root)
{
    if (!root || !root->mLeft) return root;

    TreeNode* newRoot = root->mLeft;

    std::vector<std::unique_ptr<Animation>> animationGroup;
    
    if (root->mParent)
    {
        if (root->mParent->mRight == root)
        {
            animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root->mParent, root), newRoot));
            root->mParent->mRight = newRoot;
        }
        else
        {
            animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root->mParent, root), newRoot));
            root->mParent->mLeft = newRoot;
        }
    }

    if (root->mLeft)
    {
        animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(root, root->mLeft), newRoot->mRight));
        root->mLeft = newRoot->mRight;
    }
    else
    {
        addEdge(root, newRoot->mRight);
        root->mLeft = newRoot->mRight;
    }

    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    newRoot->mParent = root->mParent;

    root->mParent = newRoot;
    root = updateHeight(root);

    if (newRoot->mRight)
    {
        animationGroup.push_back(std::make_unique<EdgeMove>(findEdge(newRoot, newRoot->mRight), root));
        newRoot->mRight = root;
    }
    else
    {
        addEdge(newRoot, root);
        newRoot->mRight = root;
    }

    newRoot = updateHeight(newRoot);
    mAnimationQueue.addAnimationGroup(animationGroup);

    align(root->mValue == mRoot->mValue ? newRoot : mRoot);
    return newRoot;
}

TreeNode* AVLTree::balance(TreeNode* root)
{
    if (!root) return nullptr;
    int bf = getBalanceFactor(root);
    if (bf > 1) {
        if (getBalanceFactor(root->mLeft) >= 0) // LL
            return rightRotate(root);
        else {
            root->mLeft = leftRotate(root->mLeft); // LR
            return rightRotate(root);
        }
    }
    if (bf < -1) {
        if (getBalanceFactor(root->mRight) <= 0) // RR
            return leftRotate(root);
        else {
            root->mRight = rightRotate(root->mRight); // RL
            return leftRotate(root);
        }
    }   
    return root;
}

void AVLTree::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mRoot) mRoot->draw(target, states);
    for (auto &edge: edges)
        if (edge)
        // edge.draw(target, states);
        target.draw(*edge, sf::Transform::Identity);
}

void AVLTree::updateCurrent(sf::Time dt)   
{
    mAnimationQueue.update(dt);
    if (mRoot) mRoot->update(dt);

    for (auto &edge: edges)
        if (edge) edge->update(dt);
}


void AVLTree::leftRotate()
{
    // mRoot->mLeft = leftRotate(mRoot->mLeft);
    mRoot = leftRotate(mRoot);
    align(mRoot);
}

void AVLTree::rightRotate()
{
    mRoot = rightRotate(mRoot);
    // align(mRoot);
}

void AVLTree::align(TreeNode* root)
{
    // return;  

    if (!root) return;
    std::vector<std::unique_ptr<Animation>> animationGroup;
    sf::Vector2f curPos = sf::Vector2f(600, 100);
    std::queue<std::pair<TreeNode*, sf::Vector2f>> q;
    q.push(std::make_pair(root, sf::Vector2f(600, 100)));
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
                {
                    curPos = prevPos + sf::Vector2f(-1200 / (1 << (cur->mLevel + 1)), 100);
                    animationGroup.push_back(std::make_unique<NodeMove>(cur, curPos, 0.5f));
                    // animationGroup.push_back(std::make_unique<EdgeMove>(cur->mParent->mLeftEdge, curPos - prevPos, 0.5f));
                }
                else // if current node is right child
                {
                    curPos = prevPos + sf::Vector2f(1200 / (1 << (cur->mLevel + 1)), 100);
                    animationGroup.push_back(std::make_unique<NodeMove>(cur, curPos, 0.5f));
                    // animationGroup.push_back(std::make_unique<EdgeMove>(cur->mParent->mRightEdge, curPos - prevPos, 0.5f));
                }
            }
            else
            {
                cur->mLevel = 0;
                animationGroup.push_back(std::make_unique<NodeMove>(cur, sf::Vector2f(600, 100), 0.5f));
                curPos = prevPos;
            }

            if (cur->mLeft) q.push(std::make_pair(cur->mLeft, curPos));
            else 
            {
                // animationGroup.push_back(std::make_unique<EdgeMove>(cur->mLeftEdge, sf::Vector2f(0, 0), 0.5f));
            }

            if (cur->mRight) q.push(std::make_pair(cur->mRight, curPos));
            else 
            {
                // animationGroup.push_back(std::make_unique<EdgeMove>(cur->mRightEdge, sf::Vector2f(0, 0), 0.5f));
            }
        }
    }
    mAnimationQueue.addAnimationGroup(animationGroup);
}

void AVLTree::addEdge(TreeNode* parent, TreeNode* child) {
    if (!findEdge(parent, child)) {
        edges.push_back(std::make_unique<Edge>(sf::Color::Black, parent, child));
    }
}

Edge* AVLTree::findEdge(TreeNode* parent, TreeNode* child) {
    for (auto& edge : edges) {
        if (edge->mFrom == parent && edge->mTo == child) {
            return edge.get();
        }
    }
    return nullptr;
}

void AVLTree::removeEdge(TreeNode* parent, TreeNode* child) {
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [parent, child](const std::unique_ptr<Edge>& edge) {
            return edge->mFrom == parent && edge->mTo == child;
        }),
        edges.end());
}

void AVLTree::changeEdgeTail(Edge* edge, TreeNode* newTail)
{
    if (!edge) return;
    edge->mTo = newTail;
    // mAnimationQueue.push
}