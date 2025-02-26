#include <DataStructures/AVLTree.hpp>

void AVLTree::insert(int value)
{
    mRoot = insert(mRoot, nullptr, value);
}

// void AVLTree::remove(int value)
// {
//     mRoot = remove(mRoot, value);
// }

// bool AVLTree::search(int value)
// {
//     return search(mRoot, value);
// }

TreeNode* AVLTree::insert(TreeNode* node, TreeNode* prev, int value)
{
    if (!node)
    {
        node = new TreeNode(value, 20.f, sf::Color::White, sf::Color::Black);
        node->setParent(prev);
        if (prev)
        {
            if (prev->getValue() > value)
                prev->attachLeft(node);
            else
                prev->attachRight(node);
        }
        return node;
    }
    if (value < node->getValue())
        node->setLeft(insert(node->getLeft(), node, value));
    
    if (value > node->getValue())
        node->setRight(insert(node->getRight(), node, value));

    return node;
}

void AVLTree::

void AVLTree::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (mRoot) target.draw(*mRoot, states);
}

void AVLTree::updateCurrent(sf::Time dt)
{
    if (mRoot) mRoot->update(dt);
}
