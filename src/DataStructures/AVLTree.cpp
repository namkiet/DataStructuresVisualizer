#include <DataStructures/AVLTree.hpp>
#include <queue>
#include <iostream>

AVLTree::AVLTree(): mRoot(nullptr) {}

void AVLTree::insert(int value)
{
    resetHistory();

    mCode = {
        "insert x",
        "check bf of this and its children",
        "  LL: this.rotR",
        "  LR: this.left.rotL, this.rotR",
        "  RR: this.rotL",
        "  RL: this.right.rotR, this.rotL",
        "this is balanced"
    };

    mActionQueue.pushInstantAction([=](){ if (!isUpdating) mStep = 0; });
    mLastStep = 6;
    mLastInfo = "Insertion is complete.";

    insertHelper(mRoot, nullptr, value);
    align(mRoot);
}

void AVLTree::remove(int value)
{
    resetHistory();

    mCode = {
        "remove x",
        "check bf of this and its children",
        "  LL: this.rotR",
        "  LR: this.left.rotL, this.rotR",
        "  RR: this.rotL",
        "  RL: this.right.rotR, this.rotL",
        "this is balanced"
    };

    mActionQueue.pushInstantAction([=](){ if (!isUpdating) mStep = 0; });
    mLastStep = 6;
    mLastInfo = "Deletion is complete.";

    removeHelper(mRoot, value);
    align(mRoot);
}

bool AVLTree::search(int value)
{
    resetHistory();

    mCode = {
        "if this == null: return false",
        "else if search value < this.key",
        "  search left",
        "else if search value > this.key:",
        "  search right",
        "else: return true"
    };

    mLastInfo = "Deletion is complete.";
    if (searchHelper(mRoot, value)) // nếu value có trong cây
    {
        mLastStep = 5;
        return true;
    }
    
    mLastStep = 0;
    return false;
}

void AVLTree::updateValue(int value, int newValue)
{
    if (!mRoot) return;

    resetHistory();

    TreeNode* cur = mRoot;
    while (cur) // find value
    {
        if (value < cur->mValue) cur = cur->mLeft;
        else if (value > cur->mValue) cur = cur->mRight;
        else break;
    }

    if (cur)
    {
        isUpdating = true;
    }

    mActionQueue.pushInstantAction([=]() { mStep = 0; });
    remove(value);
    mCode = {
        "search for x",
        "if search(x) == false: return",
        "else: remove(x); insert(y)"
    };  

    if (cur)
    {
        mActionQueue.pushInstantAction([=]() { mStep = 2; }, true);
        insert(newValue);
        mCode = {
            "search for x",
            "if search(x) == false: return",
            "else: remove(x); insert(y)"
        };  
        mLastInfo = "Updated sucessfully.";
    }
    else
    {
        mActionQueue.pushInstantAction([=]() { mStep = 1; }, true);
        mLastInfo = "Cannot find x.";
    }

    mActionQueue.pushInstantAction([=]() { isUpdating = false; }, true);
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
        node = new TreeNode(value, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
        node->mParent = prev;
        node->setOpacity(0);

        addNode(node);
        addEdge(node, nullptr);
        addEdge(node, nullptr);

        mActionQueue.pushInstantAction([=]() {
            mInfo = "Location found, inserting " + std::to_string(value) + ".";
        }, false);
        createNewActionGroup();
        mActionQueue.pushAction(Action::FadeInNode(node, 0.3f));

        if (prev) // Not root node
        {
            node->setPosition(ANIMATION::Speed < 1000 ? prev->mTargetPosition : prev->getPosition());
            node->mLevel = prev->mLevel + 1;

            // Calculate current position
            sf::Vector2f curPos = prev->mTargetPosition;
            if (value <= prev->mValue) // is left child
                curPos += sf::Vector2f(-VIZ::DS::Size.x / (1 << (node->mLevel + 1)), VIZ::DS::RowSpacing);
            else // is right child
                curPos += sf::Vector2f(VIZ::DS::Size.x / (1 << (node->mLevel + 1)), VIZ::DS::RowSpacing);
            
            // node->mTargetPosition = curPos;
            if (ANIMATION::Speed < 1000) 
                moveNode(node, curPos, 0.3f);
            // else
            //     node->mTargetPosition = curPos;

            createNewActionGroup();
            moveEdge(prev, nullptr, node, 0.3f);
        }
        else
        {
            node->mLevel = 0;
            node->mTargetPosition = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing);
            node->setPosition(node->mTargetPosition);
        }

        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " has been inserted.";
        }, false);
        return;
    }
    
    mActionQueue.pushInstantAction([=]() {
        mInfo = "Comparing " + std::to_string(value) + " with " + std::to_string(node->mValue);
    });

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.3f, false);

    if (value <= node->mValue) // Left traveral
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is less than " + std::to_string(node->mValue) + ", go left.";
        });
        if (node->mLeft)
        {
            createNewActionGroup();
            traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);
        }
        insertHelper(node->mLeft, node, value);
    }
    else if (value > node->mValue) // Right traversal
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is greater than " + std::to_string(node->mValue) + ", go right.";
        });
        if (node->mLeft)
        {
            createNewActionGroup();
            traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);
        }
        insertHelper(node->mRight, node, value);
    }

    createNewActionGroup();
    highlightNode(node, VIZ::NODE::FillColor, 0.3f, false);

    node = updateHeight(node);
    balance(node);

    mActionQueue.pushInstantAction([=]() {
        mInfo = "Check the parent.";
    });
}

void AVLTree::removeHelper(TreeNode* &node, int value)
{
    if (!node) 
    {
        mLastInfo = std::to_string(value) + " is not in the tree.";
        return;
    }

    mActionQueue.pushInstantAction([=]() {
        mInfo = "Comparing " + std::to_string(value) + " with " + std::to_string(node->mValue);
    });

    createNewActionGroup();
    highlightNode(node, sf::Color::Red, 0.3f);

    if (value < node->mValue) // Left traveral
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is less than " + std::to_string(node->mValue) + ", go left.";
        });
        createNewActionGroup();
        if (node->mLeft)
        {
            createNewActionGroup();
            traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);
        }
        removeHelper(node->mLeft, value);
    }

    else if (value > node->mValue) // Right traversal
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is greater than " + std::to_string(node->mValue) + ", go right.";
        });
        if (node->mRight)
        {
            createNewActionGroup();
            traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);
        }
        removeHelper(node->mRight, value);
    }
    else 
    {
        if (isUpdating)
            mActionQueue.pushInstantAction([=]() { mStep = 2; });

        if (!node->mLeft || !node->mRight) {

            TreeNode* temp = node->mLeft ? node->mLeft : node->mRight;

            if (temp)
            {
                temp->mParent = node->mParent;
                std::string s = (temp == node->mLeft ? "left" : "right");
                mActionQueue.pushInstantAction([=]() {
                    mInfo = "Node " + std::to_string(value) + " only has a " + s + " child.";
                }, true);
                mActionQueue.pushInstantAction([=]() {
                    mInfo = "Remove node " + std::to_string(value) + " and connect its parent to its " + s + " child.";
                });
            }
            else
            {
                mActionQueue.pushInstantAction([=]() {
                    mInfo = "Node " + std::to_string(value) + " is a leaf.";
                }, true);
                mActionQueue.pushInstantAction([=]() {
                    mInfo = "Remove leaf " + std::to_string(value) + ".";
                });
            }

            createNewActionGroup();
            highlightNode(node, VIZ::NODE::FillColor, 0.3f, false);
            mActionQueue.pushAction(Action::FadeOutNode(node, 0.3f));

            if (node->mParent)
            {
                moveEdge(node->mParent, node, temp, 0.3f);
            }

            if (temp) 
            {
                moveEdge(node, temp, nullptr, 0.3f);
                createNewActionGroup();
                mActionQueue.pushAction(Action::MoveNode(temp, node->getPosition(), 0.3f));
            }


            createNewActionGroup();
            removeNode(node); // Remove a node and all edges FROM it

            node = temp;

            mActionQueue.pushInstantAction([=]() {
                mInfo = "Removal of " + std::to_string(value) + " is complete.";
            });



            return;
        }
        else
        {
            mActionQueue.pushInstantAction([=](){
                mInfo = "Finding successor of " + std::to_string(value) + ".";
            });

            TreeNode* cur = node->mRight;
            while (cur->mLeft)
            {
                createNewActionGroup();
                highlightNode(cur, sf::Color::Yellow, 0.3f);
                cur = cur->mLeft;
            }
            createNewActionGroup();
            highlightNode(cur, sf::Color::Yellow, 0.3f);

            mActionQueue.pushInstantAction([=](){
                mInfo = "Replace node " + std::to_string(value) + " with its successor.";
            });

            createNewActionGroup();
            swapTwoNodes(cur, node, 0.3f);
            removeHelper(node->mRight, value);
        }
    }

    node = updateHeight(node);
    balance(node);
}

bool AVLTree::searchHelper(TreeNode* node, int value)
{
    if (!node) 
    {
        mActionQueue.pushInstantAction([=]() {
            mInfo = std::to_string(value) + " is not in the tree.";
        }); 
        return false;
    }
    
    mActionQueue.pushInstantAction([=]() {
        mInfo = "Comparing " + std::to_string(value) + " with " + std::to_string(node->mValue);
    });
    highlightNode(node, sf::Color::Red, 0.3f);

    if (value < node->mValue) 
    {
        mActionQueue.pushInstantAction([=]() { mStep = 1; }, true);
        mActionQueue.pushInstantAction([=]() { 
            mStep = 2; 
            mInfo = std::to_string(value) + " is less than " + std::to_string(node->mValue) + ", go left.";
        });
        if (node->mLeft)
        {
            createNewActionGroup();
            traverseEdge(node, node->mLeft, sf::Color::Red, 0.3f);
        }
        return searchHelper(node->mLeft, value);
    }
    else if (value > node->mValue)
    {
        mActionQueue.pushInstantAction([=]() { mStep = 3; }, true);

        mActionQueue.pushInstantAction([=]() { 
            mStep = 4;
            mInfo = std::to_string(value) + " is greater than " + std::to_string(node->mValue) + ", go right.";
        });
        if (node->mLeft)
        {
            createNewActionGroup();
            traverseEdge(node, node->mRight, sf::Color::Red, 0.3f);
        }
        return searchHelper(node->mRight, value);
    }
    else
    {
        mActionQueue.pushInstantAction([=]() { 
            mStep = 5; 
            mInfo = std::to_string(value) + " is found.";
        }, true);
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

void AVLTree::balance(TreeNode* &root)
{
    if (!root) return;

    int bf = getBalanceFactor(root);
    mActionQueue.pushInstantAction([=]() {
        if (!isUpdating) mStep = 1;
        root->setNote("bf = " + std::to_string(bf));
        mInfo = "Balance factor of " + std::to_string(root->mValue) + " is " + std::to_string(bf);
    });
    highlightNode(root, sf::Color::Blue, 0.3f);

    mActionQueue.pushInstantAction([=]() {
        root->setNote("");
    });

    if (bf > 1) 
    {
        if (getBalanceFactor(root->mLeft) >= 0) // LL
        {
            mActionQueue.pushInstantAction([=]() { if (!isUpdating) mStep = 4; });
            root = rightRotate(root);
        }
        else 
        {
            mActionQueue.pushInstantAction([=]() { if (!isUpdating) mStep = 5; });
            root->mLeft = leftRotate(root->mLeft); // LR
            root = rightRotate(root);
        }
    }
    else if (bf < -1) 
    {
        if (getBalanceFactor(root->mRight) <= 0) // RR
        {
            mActionQueue.pushInstantAction([=]() { if (!isUpdating) mStep = 2; });
            root = leftRotate(root);
        }
        else 
        {
            mActionQueue.pushInstantAction([=]() { if (!isUpdating) mStep = 3; });
            root->mRight = rightRotate(root->mRight); // RL
            root = leftRotate(root);
        }
    }
}

TreeNode* AVLTree::leftRotate(TreeNode* root)
{
    if (!root || !root->mRight) return root;

     
    mActionQueue.pushInstantAction([=]() {
        mInfo = "Rotate left at " + std::to_string(root->mValue);
    }, true);
    
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

    
    mActionQueue.pushInstantAction([=]() {
        mInfo = "Rotate left at " + std::to_string(root->mValue);
    }, true);

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

void AVLTree::align(TreeNode* curNode, sf::Vector2f curPos, float curSpacingX, float curSpacingY) // DFS
{
    if (!curNode || ANIMATION::Speed >= 1000) return;

    if (!curNode->mParent)
    {
        curNode->mLevel = 0;
        createNewActionGroup();
    }
    else
    {
        curNode->mLevel = curNode->mParent->mLevel + 1;
    }

    if (curNode->mTargetPosition != curPos) 
    {
        std::cerr << curNode->mValue << ": " << curNode->getPosition().x << " " << curNode->getPosition().y << " " << curPos.x << " " << curPos.y  << "\n";
        moveNode(curNode, curPos, 0.3f);
    }
    
    // DFS down to its children
    sf::Vector2f leftChildPos = curPos + sf::Vector2f(-curSpacingX, curSpacingY);
    sf::Vector2f rightChildPos = curPos + sf::Vector2f(curSpacingX, curSpacingY);

    float newSpacingX = curSpacingX / 2;
    float newSpacingY = curSpacingY;

    align(curNode->mLeft, leftChildPos, newSpacingX, newSpacingY);
    align(curNode->mRight, rightChildPos, newSpacingX, newSpacingY);
}