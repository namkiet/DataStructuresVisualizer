#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/CircleNode.hpp>

class Edge :
    public sf::Transformable, public sf::Drawable
{
public:
    typedef std::shared_ptr<Edge> Ptr;

public:
                                Edge(sf::Color color, CircleNode* from, CircleNode* to, bool hasArrow, float thickness);
                                Edge(sf::Color color, CircleNode* from, CircleNode* to, int weight, bool hasArrow, float thickness);
                                
    virtual void                draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void                update(sf::Time dt);
    // void                        setWeight(int weight);
    int                         getWeight();
    void                        updateEdge();


public:
    CircleNode*                 mFrom;
    CircleNode*                 mTo;
    bool                        mIsChangingTail;

private:
    float                       mThickness;
    sf::Vector2f                mHead;
    sf::Vector2f                mMid;
    sf::Vector2f                mTail;
    sf::Color                   mColor;

    
    sf::VertexArray             mLine;
    bool                        mHasArrow;

    sf::Color                   mMarkColor;
    sf::VertexArray             mLine1; // mFrom to mMid
    sf::VertexArray             mLine2; // mMid to mTo
    bool                        isReversed;

private:
    float                       mArrowSize;
    sf::ConvexShape             mArrowHead;

private:
    bool                        mHasWeight;
    int                         mWeight;
    sf::Text                    mWeightText;
    sf::Font                    mFont;
    


public:
    void                        setHead(sf::Vector2f head);
    void                        setMid(sf::Vector2f mid);
    void                        setTail(sf::Vector2f tail);
    void                        setColor(sf::Color color);
    void                        setThickness(float thickness);
    sf::Vector2f                getHead();
    sf::Vector2f                getTail();
    sf::Color                   getColor();
    void                        resetColor();
    sf::Color                   getMarkColor();
    void                        resetMid();
    double                      lengthEdge();
    void                        swapEndpoint();
}; 