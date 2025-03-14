#include <DataStructures/HeapTree.hpp>
#include <Core/Animation.hpp>
#include <queue>
#include <iostream>

HeapTree::HeapTree():
    mRoot(nullptr)
{    
}

void HeapTree::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // if (mRoot) mRoot->draw(target, states);

    for (auto node: mHeapArray)
        node->draw(target, states);
}

void HeapTree::updateCurrent(sf::Time dt)   
{
    mAnimationQueue.update(dt);
    // if (mRoot) mRoot->update(dt);

    for (auto node: mHeapArray)
        node->update(dt);
}

void HeapTree::insert(int value)
{
    // mRoot = insert(mRoot, nullptr, value);
    // align(mRoot);

    TreeNode* newNode = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
    mHeapArray.push_back(newNode);

    // for (auto node: mHeapArray)
    //     std::cerr << node->mValue << " ";
    // std::cerr << "\n";


    int n = mHeapArray.size() - 1;
    // if (n)
    // {        
    //     TreeNode* par = mHeapArray[(n - 1) / 2];
    //     newNode->mParent = par;
    //     if (par->mLeft)
    //     {
    //         std::cerr << par->mLeft->mValue << " ";
    //         par->mRight = newNode;
    //     }
    //     else par->mLeft = newNode;
    // }

    // align();


    while (n)
    {
        if (mHeapArray[(n - 1)/ 2]->mValue < mHeapArray[n]->mValue)
        {
            std::vector<std::unique_ptr<Animation>> animationGroup;

            // mHeapArray[n]->mLeft = mHeapArray[(n - 1)/ 2]->mLeft;
            // mHeapArray[n]->mRight = mHeapArray[(n - 1)/ 2]->mRight;

            // mHeapArray[(n - 1) / 2]->mParent = mHeapArray[n]->mParent;
            
            // sf::Vector2f vec1 = mHeapArray[n]->getPosition();
            // sf::Vector2f vec2 = mHeapArray[(n - 1) / 2]->getPosition();

            // animationGroup.push_back(std::make_unique<NodeMove>(mHeapArray[(n - 1)/ 2], vec1, 2.5f));
            // animationGroup.push_back(std::make_unique<NodeMove>(mHeapArray[n], vec2, 2.5f));
            // mAnimationQueue.addAnimationGroup(animationGroup);

            std::swap(mHeapArray[(n - 1)/  2], mHeapArray[n]);
            n = (n - 1) / 2;
        }
        else break;
    }

    for (auto node: mHeapArray)
        std::cerr << node->mValue << " ";
    std::cerr << "\n";

    align();    


    // align(mHeapArray[0]);

}

// void HeapTree::remove(int value)
// {
//     mRoot = remove(mRoot, value);
// }

bool HeapTree::search(int value)
{
    return search(mRoot, value);
}

TreeNode* HeapTree::insert(TreeNode* node, TreeNode* prev, int value)
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

    return node;
}

bool HeapTree::search(TreeNode* node, int value)
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


// void HeapTree::align(TreeNode* root)
// {
//     if (!root) return;
//     std::vector<std::unique_ptr<Animation>> animationGroup;
//     sf::Vector2f curPos = sf::Vector2f(600, 100);
//     std::queue<std::pair<TreeNode*, sf::Vector2f>> q;
//     q.push(std::make_pair(root, sf::Vector2f(600, 100)));
//     while (!q.empty())
//     {
//         int levelSize = q.size();
//         for (int i = 0; i < levelSize; i++)
//         {
//             TreeNode* cur = q.front().first;
//             sf::Vector2f prevPos = q.front().second;
//             q.pop();

//             sf::Vector2f curPos;
//             if (cur->mParent)
//             {
//                 // std::cerr << cur->mValue << ": " << cur->mParent->mValue << " " << cur->mParent->getPosition().x << " " << cur->mParent->getPosition().y << "\n";
//                 cur->mLevel = cur->mParent->mLevel + 1;
//                 if (cur->mParent->mLeft == cur) // if current node is left child
//                 {
//                     curPos = prevPos + sf::Vector2f(-1200 / (1 << (cur->mLevel + 1)), 100);
//                     animationGroup.push_back(std::make_unique<NodeMove>(cur, curPos, 0.5f));
//                     animationGroup.push_back(std::make_unique<EdgeMove>(cur->mParent->mLeftEdge, curPos - prevPos, 0.5f));
//                 }
//                 else // if current node is right child
//                 {
//                     curPos = prevPos + sf::Vector2f(1200 / (1 << (cur->mLevel + 1)), 100);
//                     animationGroup.push_back(std::make_unique<NodeMove>(cur, curPos, 0.5f));
//                     animationGroup.push_back(std::make_unique<EdgeMove>(cur->mParent->mRightEdge, curPos - prevPos, 0.5f));
//                 }
//             }
//             else
//             {
//                 cur->mLevel = 0;
//                 animationGroup.push_back(std::make_unique<NodeMove>(cur, sf::Vector2f(600, 100), 0.5f));
//                 curPos = prevPos;
//             }

//             if (cur->mLeft) q.push(std::make_pair(cur->mLeft, curPos));
//             else 
//                 animationGroup.push_back(std::make_unique<EdgeMove>(cur->mLeftEdge, sf::Vector2f(0, 0), 0.5f));

//             if (cur->mRight) q.push(std::make_pair(cur->mRight, curPos));
//             else 
//                 animationGroup.push_back(std::make_unique<EdgeMove>(cur->mRightEdge, sf::Vector2f(0, 0), 0.5f));
//         }
//     }
//     mAnimationQueue.addAnimationGroup(animationGroup);
// }

void HeapTree::align()
{
    std::vector<std::unique_ptr<Animation>> animationGroup;

    for (int i = 0; i < mHeapArray.size(); i++)
    {
        if (i == 0)
        {
            mHeapArray[0]->setPosition(sf::Vector2f(600, 100));
            mHeapArray[0]->mLevel = 0;
        }
        else
        {
            std::cerr << mHeapArray[i]->mValue << " " << mHeapArray[(i - 1) / 2]->mValue << "\n";
            // int parent = (i - 1) / 2;
            mHeapArray[i]->mLevel = mHeapArray[(i - 1) / 2]->mLevel + 1;
            if (i % 2 == 1)
                animationGroup.push_back(std::make_unique<NodeMove>(mHeapArray[i], sf::Vector2f(mHeapArray[(i - 1) / 2]->getPosition() + sf::Vector2f(-1200 / (1 << (mHeapArray[i]->mLevel + 1)), 100)), 0.5f));
                // mHeapArray[i]->setPosition(sf::Vector2f(mHeapArray[(i - 1) / 2]->getPosition() + sf::Vector2f(-1200 / (1 << (mHeapArray[i]->mLevel + 1)), 100)));
            else
                animationGroup.push_back(std::make_unique<NodeMove>(mHeapArray[i], sf::Vector2f(mHeapArray[(i - 1) / 2]->getPosition() + sf::Vector2f(1200 / (1 << (mHeapArray[i]->mLevel + 1)), 100)), 0.5f));
                // mHeapArray[i]->setPosition(sf::Vector2f(mHeapArray[(i - 1) / 2]->getPosition() + sf::Vector2f(1200 / (1 << (mHeapArray[i]->mLevel + 1)), 100)));
            
            mAnimationQueue.addAnimationGroup(animationGroup);
        }
    }
}