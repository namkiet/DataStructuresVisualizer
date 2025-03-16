#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/CircleNode.hpp>

class Edge :
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
    typedef std::unique_ptr<Edge> Ptr;

public:
                                Edge(sf::Color color, CircleNode* from, CircleNode* to, bool hasArrow);
    virtual void                draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void                update(sf::Time dt);


public:
    CircleNode*                 mFrom;
    CircleNode*                 mTo;
    bool                        mIsChangingTail;

private:
    sf::Vector2f                mHead;
    sf::Vector2f                mTail;
    sf::Color                   mColor;
    sf::VertexArray             mLine;

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