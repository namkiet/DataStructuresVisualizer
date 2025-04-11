#include <DataStructures/AVLTree.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree(): mRoot(nullptr) {}

void AVLTree::insert(int value)
{
    execute();

    mCode = {
        "insert x",
        "check bf of this and its children",
        "  LL: this.rotR",
        "  LR: this.left.rotL, this.rotR",
        "  RR: this.rotL",
        "  RL: this.right.rotR, this.rotL",
        "this is balanced"
    };

    mStep = 0;
    mLast = 6;

    insertHelper(mRoot, nullptr, value);
    align(mRoot);
}

void AVLTree::remove(int value)
{
    // saveState();
    
    mCode = {
        "remove x",
        "check bf of this and its children",
        "  LL: this.rotR",
        "  LR: this.left.rotL, this.rotR",
        "  RR: this.rotL",
        "  RL: this.right.rotR, this.rotL",
        "this is balanced"
    };

    mStep = 0;
    mLast = 6;

    removeHelper(mRoot, value);
    mActionQueue.pushInstantAction([&]() { align(mRoot); });
}

bool AVLTree::search(int value)
{
    mCode = {
        "if this == null: return false",
        "else if search value < this.key",
        "  search left",
        "else if search value > this.key:",
        "  search right",
        "else: return true"
    };

    if (searchHelper(mRoot, value)) // nếu value có trong cây
    {
        mLast = 5;
        return true;
    }
    
    mLast = 0;
    return false;
}

void AVLTree::empty()
{
    DS::empty();
    mRoot = nullptr;
}

void AVLTree::insertHelper(TreeNode* &node, TreeNode* prev, int value)
{
    if (!node)
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = "Location found, inserting " + std::to_string(value) + ".\n";
        });

        node = new TreeNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
        node->mParent = prev;
        node->setOpacity(0);


        createNewActionGroup();
        addNode(node);
        addEdge(node, nullptr);
        addEdge(node, nullptr);

        // if (ANIMATION::Speed < 100)
        // {
            createNewActionGroup();
            mActionQueue.pushAction(Action::FadeInNode(node, 0.3f));
        // }

        if (prev) // Not root node
        {
            node->setPosition(prev->getPosition());
            node->mLevel = prev->mLevel + 1;

            // Calculate current position
            sf::Vector2f curPos = prev->getPosition();
            if (value < prev->mValue) // is left child
                curPos += sf::Vector2f(-VIZ::DS::Size.x / (1 << (node->mLevel + 1)), VIZ::DS::RowSpacing);
            else // is right child
                curPos += sf::Vector2f(VIZ::DS::Size.x / (1 << (node->mLevel + 1)), VIZ::DS::RowSpacing);
            
            if (ANIMATION::Speed < 1000) {
                moveNode(node, curPos, 0.3f);
            }

            createNewActionGroup();
            moveEdge(prev, nullptr, node, 0.3f);
        }
        else
        {
            node->mLevel = 0;
            node->setPosition(sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing));
        }
        return;
    }
    
    mActionQueue.pushInstantAction([=]() {
        mInfo = "Comparing " + std::to_string(value) + " with " + std::to_string(node->mValue) + ".\n";
    });

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.3f, false);

    if (value < node->mValue) // Left traveral
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is less than " + std::to_string(node->mValue) + ", go left.\n";
        });
        createNewActionGroup();
        traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);
        insertHelper(node->mLeft, node, value);
    }

    if (value > node->mValue) // Right traversal
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is greater than " + std::to_string(node->mValue) + ", go right.\n";
        });
        createNewActionGroup();
        traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);
        insertHelper(node->mRight, node, value);
    }


    createNewActionGroup();
    highlightNode(node, VIZ::NODE::FillColor, 0.3f, false);

    node = updateHeight(node);
    node = balance(node);
}

void AVLTree::removeHelper(TreeNode* &node, int value)
{
    if (!node) return;

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.3f);

    if (value < node->mValue) // Left traveral
    {
        createNewActionGroup();
        traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);
        removeHelper(node->mLeft, value);
    }

    else if (value > node->mValue) // Right traversal
    {
        createNewActionGroup();
        traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);
        removeHelper(node->mRight, value);
    }
    else 
    {
        if (!node->mLeft || !node->mRight) {
            TreeNode* temp = node->mLeft ? node->mLeft : node->mRight;

            if (temp)
                temp->mParent = node->mParent;

            createNewActionGroup();
            mActionQueue.pushAction(Action::FadeOutNode(node, 0.3f));

            if (node->mParent)
            {
                moveEdge(node->mParent, node, temp, 0.3f);
            }

            moveEdge(node, temp, nullptr, 0.3f);

            createNewActionGroup();
            mActionQueue.pushAction(Action::MoveNode(temp, node->getPosition(), 0.3f));

            createNewActionGroup();
            removeNode(node); // Remove a node and all edges FROM it\

            node = temp;
        }
        else
        {
            TreeNode* cur = node->mRight;
            while (cur->mLeft)
                cur = cur->mLeft;
        
            createNewActionGroup();
            swapTwoNodes(cur, node);

            mActionQueue.pushInstantAction([=]() {
                removeHelper(node->mRight, value);
            });
        }
    }
    
    mActionQueue.pushInstantAction([&]() {
        node = updateHeight(node);
        node = balance(node);
    });
}

bool AVLTree::searchHelper(TreeNode* node, int value)
{
    if (!node) return false;

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.3f);

    if (value < node->mValue) 
    {
        mActionQueue.pushInstantAction([=]() { mStep = 1; }, true);

        mActionQueue.pushInstantAction([=]() { mStep = 2; });
        traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);

        return searchHelper(node->mLeft, value);
    }
    else if (value > node->mValue)
    {
        mActionQueue.pushInstantAction([=]() { mStep = 3; }, true);

        mActionQueue.pushInstantAction([=]() { mStep = 4; });
        traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);

        return searchHelper(node->mRight, value);
    }
    else
    {
        mActionQueue.pushInstantAction([=]() { mStep = 5; }, true);

        mActionQueue.pushInstantAction([=]() { mStep = 6; });
        int blinkTimes = 3;
        while (blinkTimes--)
        {
            createNewActionGroup();
            highlightNode(node, sf::Color::Blue, 0.25f);
        }
        return true;
    }

    return true;
}

int AVLTree::getHeight(TreeNode* root) 
{
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
        moveEdge(par, root, newRoot, 0.3f);
        if (par->mLeft == root) 
            par->mLeft = newRoot;
        else 
            par->mRight = newRoot;
    }

    if (newRoot->mLeft) 
        newRoot->mLeft->mParent = root;

    moveEdge(root, root->mRight, newRoot->mLeft, 0.3f);
    root->mRight = newRoot->mLeft;

    moveEdge(newRoot, newRoot->mLeft, root, 0.3f);
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
        moveEdge(par, root, newRoot, 0.3f);
        if (par->mRight == root) 
            par->mRight = newRoot;
        else 
            par->mLeft = newRoot;
    }

    if (newRoot->mRight) 
        newRoot->mRight->mParent = root;

    moveEdge(root, root->mLeft, newRoot->mRight, 0.3f);
    root->mLeft = newRoot->mRight;

    moveEdge(newRoot, newRoot->mRight, root, 0.3f);
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

    mActionQueue.pushInstantAction([=]() {
        mStep = 1;
        root->setNote("bf = " + std::to_string(bf));
    });


    highlightNode(root, sf::Color::Blue, 0.3f);

    mActionQueue.pushInstantAction([=]() {
        root->setNote("");
    });

    if (bf > 1) 
    {
        if (getBalanceFactor(root->mLeft) >= 0) // LL
        {
            mActionQueue.pushInstantAction([=]() { mStep = 4; });
            return rightRotate(root);
        }
        else 
        {
            mActionQueue.pushInstantAction([=]() { mStep = 5; });
            root->mLeft = leftRotate(root->mLeft); // LR
            return rightRotate(root);
        }
    }
    if (bf < -1) 
    {
        if (getBalanceFactor(root->mRight) <= 0) // RR
        {
            mActionQueue.pushInstantAction([=]() { mStep = 2; });
            return leftRotate(root);
        }
        else 
        {
            mActionQueue.pushInstantAction([=]() { mStep = 3; });
            root->mRight = rightRotate(root->mRight); // RL
            return leftRotate(root);
        }
    }

    return root;
}

void AVLTree::align(TreeNode* curNode, sf::Vector2f curPos, float curSpacingX, float curSpacingY) // DFS
{
    if (!curNode) return;

    if (ANIMATION::Speed >= 100) return;

    if (!curNode->mParent)
    {
        curNode->mLevel = 0;
        createNewActionGroup();
    }
    else
        curNode->mLevel = curNode->mParent->mLevel + 1;
    
    std::cerr << curNode->mValue << ": " << curNode->getOpacity() << "\n";

    moveNode(curNode, sf::Vector2f(int(curPos.x), int(curPos.y)), 0.3f);
    
    // DFS down to their children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(curNode->mLeft, leftChildPos, newSpacingX, newSpacingY);
    align(curNode->mRight, rightChildPos, newSpacingX, newSpacingY);
}

void AVLTree::saveState(std::stack<History> &stack) {
    if (ANIMATION::Speed >= 1000) return;

    std::vector<CircleNode::Ptr> savedNodeList;
    std::vector<Edge::Ptr> savedEdgeList;

    if (!mRoot) {
        stack.push(History(std::move(savedNodeList), std::move(savedEdgeList), nullptr));
        return;
    }

    std::unordered_map<TreeNode*, TreeNode*> nodeMap;
    std::vector<TreeNode::Ptr> tempNodeList;
    nodeMap[nullptr] = nullptr;  // Map nullptr -> nullptr

    // Clone root node
    TreeNode* savedRoot = new TreeNode(*mRoot);
    nodeMap[mRoot] = savedRoot;
    tempNodeList.push_back(TreeNode::Ptr(savedRoot));

    // BFS clone toàn bộ cây
    std::queue<std::pair<TreeNode*, TreeNode*>> q;
    q.push({mRoot, savedRoot});

    while (!q.empty()) {
        auto [oldNode, newNode] = q.front();
        q.pop();

        TreeNode* newLeft = oldNode->mLeft ? new TreeNode(*oldNode->mLeft) : nullptr;
        TreeNode* newRight = oldNode->mRight ? new TreeNode(*oldNode->mRight) : nullptr;

        newNode->mLeft = newLeft;
        newNode->mRight = newRight;
        if (newLeft) newLeft->mParent = newNode;
        if (newRight) newRight->mParent = newNode;

        // nodeMap[oldNode->mLeft] = newLeft;
        // nodeMap[oldNode->mRight] = newRight; 

        savedEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, newNode, newNode->mLeft, false, 1.5f));
        savedEdgeList.push_back(std::make_unique<Edge>(VIZ::EDGE::Color, newNode, newNode->mRight, false, 1.5f));

        if (newLeft) {
            tempNodeList.push_back(TreeNode::Ptr(newLeft));
            q.push({oldNode->mLeft, newLeft});
        }
        if (newRight) {
            tempNodeList.push_back(TreeNode::Ptr(newRight));
            q.push({oldNode->mRight, newRight});
        }
    }

    // Chuyển tempNodeList từ TreeNode::Ptr sang CircleNode::Ptr
    for (auto& node : tempNodeList)
        savedNodeList.push_back(std::move(node));

    // auto savedRoot = static_cast<TreeNode*>(savedNodeList.front().get());

    stack.push(History(std::move(savedNodeList), std::move(savedEdgeList), savedRoot));

    std::cerr << "VCL: " << stack.size() << "\n";
}

void AVLTree::loadState(History history)
{
    empty();

    mNodeList = std::move(history.nodeList);
    mEdgeList = std::move(history.edgeList);
    mRoot = static_cast<TreeNode*>(history.baseNode);
}