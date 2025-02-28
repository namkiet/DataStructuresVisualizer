#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>

class TreeNode :
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
                            TreeNode(int value, float radius, sf::Color fillColor, sf::Color outlineColor);

private:
    int                     mValue;
    TreeNode*               mLeft;
    TreeNode*               mRight;
    TreeNode*               mParent;
    int                     mHeight;
    int                     mLevel;
    // void                    inorder(TreeNode* node, std::vector<TreeNode*> &nodeList);
    void                    updateLevel(TreeNode* &node, int level);

public:
    bool                    isLeftChild();
    bool                    isRightChild();

public:
    int                     getValue();
    TreeNode*               getLeft();
    TreeNode*               getRight();
    TreeNode*               getParent();
    int                     getHeight();
    int                     getLevel();

    void                    setLeft(TreeNode* node);
    void                    setRight(TreeNode* node);
    void                    setParent(TreeNode* node);
    void                    setHeight(int height);
    void                    setLevel(int level);
    void                    updateHeight();
    void                    updateLevel();

public:
    TreeNode*               insert(TreeNode* node, TreeNode* prev, int value);
    bool                    search(TreeNode* node, int value);

    int                     get_Height(TreeNode* root);
    TreeNode*               update_Height(TreeNode* root);
    int                     get_bf(TreeNode* root);
    TreeNode*               leftRotate(TreeNode* x);
    TreeNode*               rightRotate(TreeNode* y);
    TreeNode*               balance(TreeNode* root);

private:
    virtual void            updateCurrent(sf::Time dt);
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    virtual void            update(sf::Time dt);
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    bool                    mIsAnimationFinished;

private:
    bool                    mIsMoving;
    sf::Vector2f            mTargetPosition;

public:
    void                    moveTo(sf::Vector2f target);

private:
    bool                    mIsHighlighted;
    sf::Color               mTargetColor;
    float                   mElapsedTime;
    float                   mDuration;

public:
    void                    highlight(sf::Color targetColor, float elapsedTime, float duration);

private:
    sf::CircleShape         mShape;
    sf::Text                mText;
    sf::Font                font;
};