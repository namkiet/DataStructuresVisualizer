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

public:
    int                     getValue();
    TreeNode*               getLeft();
    TreeNode*               getRight();
    TreeNode*               getParent();

    void                    setLeft(TreeNode* node);
    void                    setRight(TreeNode* node);
    void                    setParent(TreeNode* node);

private:
    sf::CircleShape         mShape;
    sf::Text                mText;

    sf::Font font;

private:
    virtual void            updateCurrent(sf::Time dt);

private:
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    virtual unsigned int    getCategory() const;

private:
    bool                    mIsMoving;
    sf::Vector2f            mTargetPosition;

public:
    void                    moveTo(sf::Vector2f target);

public:
    void attachLeft(TreeNode* child) {
        child->moveTo(sf::Vector2f(-100, 100));
        mLeft = child;
        SceneNode::attachChild(std::unique_ptr<SceneNode>(child));
        // delete child;
    }

    void attachRight(TreeNode* child) {
        child->moveTo(sf::Vector2f(100, 100));
        mRight = child;
        SceneNode::attachChild(std::unique_ptr<SceneNode>(child));
        // delete child;
    }
};