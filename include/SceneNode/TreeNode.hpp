#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>

class TreeNode : public SceneNode
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
    void                    inorder(TreeNode* node, std::vector<TreeNode*> &nodeList);

public:
    int                     getValue();
    TreeNode*               getLeft();
    TreeNode*               getRight();
    TreeNode*               getParent();
    int                     getHeight();
    int                     getLevel();
    std::vector<TreeNode*>  getInorderTraversal(TreeNode* node);

    void                    setLeft(TreeNode* node);
    void                    setRight(TreeNode* node);
    void                    setParent(TreeNode* node);
    void                    updateHeight();
    void                    updateLevel();

private:
    sf::CircleShape         mShape;
    sf::Text                mText;

    sf::Font font;

private:
    virtual void            updateCurrent(sf::Time dt);

public:
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    virtual unsigned int    getCategory() const;

private:
    bool                    mIsMoving;
    sf::Vector2f            mTargetPosition;

public:
    void                    moveTo(sf::Vector2f target);

public:
    void                    attachLeft(TreeNode* child);

    void                    attachRight(TreeNode* child);
};