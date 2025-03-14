#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/TreeNode.hpp>

class Edge :
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
    typedef std::unique_ptr<Edge> Ptr;

public:
                                // Edge(sf::Color color, sf::Vector2f head, sf::Vector2f tail);
                                Edge(sf::Color color, TreeNode* from, TreeNode* to);
    virtual void                draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void                update(sf::Time dt);

private:
    sf::Vector2f                mHead;
    sf::Vector2f                mTail;
    sf::Color                   mColor;
    sf::VertexArray             mLine;

public:
    TreeNode*                   mFrom;
    TreeNode*                   mTo;

public:
    bool                        mIsChangingTail;

private:
    bool                        mHasArrow;
    float                       mArrowSize;
    sf::ConvexShape             mArrowHead;

private:
    bool                        mHasWeight;
    int                         mWeight;

    void                        buildEdge();

public:
    void                        setHead(sf::Vector2f head);
    void                        setTail(sf::Vector2f tail);
    void                        setColor(sf::Color color);
    sf::Vector2f                getHead();
    sf::Vector2f                getTail();
    sf::Color                   getColor();
};