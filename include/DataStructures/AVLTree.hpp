#pragma once
#include <DataStructures/AVLNode.hpp>

class AVLTree: 
public SceneNode
// public sf::Transformable, public sf::Drawable,
//     private sf::NonCopyable
{
public:
                            AVLTree();
    AVLNode*                mRoot;

public:
    void                    insert(int value);
    // void                remove(int value);
    bool                    search(int value);

private:
    // AVLNode*               insert(AVLNode* node, AVLNode* prev, int value);
    // AVLNode*           remove(AVLNode* node, int value);
    // bool                search(AVLNode* node, int value);

    void                    updateLevel(AVLNode* node, int level);

public:
    void                    leftRotate();
    void                    rightRotate();


public:
    virtual void            updateCurrent(sf::Time dt);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    void                    align(AVLNode* node);
};