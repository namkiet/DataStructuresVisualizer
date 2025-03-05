#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/TreeNode.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>

class AVLNode : public TreeNode
{
public:
                            AVLNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

private:
    int                     mValue;
    AVLNode*                mLeft;
    AVLNode*                mRight;
    AVLNode*                mParent;
    int                     mHeight;
    int                     mLevel;
    void                    updateLevel(AVLNode* &node, int level);

public:
    bool                    isLeftChild();
    bool                    isRightChild();
    
public:
    AVLNode*                getLeft();
    AVLNode*                getRight();
    AVLNode*                getParent();
    
    void                    setLeft(AVLNode* node);
    void                    setRight(AVLNode* node);
    void                    setParent(AVLNode* node);

public:
    int                     getHeight();
    int                     getLevel();

    void                    setHeight(int height);
    void                    setLevel(int level);
    
    void                    updateHeight();
    void                    updateLevel();

public:
    AVLNode*                insert(AVLNode* node, AVLNode* prev, int value);
    bool                    search(AVLNode* node, int value);

    int                     get_Height(AVLNode* root);
    AVLNode*                update_Height(AVLNode* root);
    int                     get_bf(AVLNode* root);
    AVLNode*                leftRotate(AVLNode* x);
    AVLNode*                rightRotate(AVLNode* y);
    AVLNode*                balance(AVLNode* root);

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
};