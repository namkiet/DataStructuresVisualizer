#pragma once
#include<vector>
#include<utility>
using namespace std;
#include <DataStructures/DS.hpp>

class Graph: public DS
{
public:
                                Graph(vector<vector<pair<int,int>>> G);  
                                Graph();
    void                        Prim();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        empty() override;

    int                         EdgeID(int start, int end);

    void                        MarkEdge(Edge* edge, int direction, float duration); // direction = 1 if edge from mFrom to mTo, otherwise -1
    void                        handleEvent(const sf::Event& event); // to handle mouse events click on node or sth
    virtual void                updateCurrent(sf::Time dt);
    bool                        loadFromVector(std::vector<int> numList) override;

private:
    void                        physicalUpdate(sf::Time dt);
    bool                        checkValidInput(const std::vector<int>& data);


private:
    bool                        isReady;
    int                         NumVer;
    int                         NumEdge;
    int                         selectID;
    vector<sf::Vector2f>        velocity;
    float                       ForceConstant;

    int                         mstSum = 0;

public:
    float                       loadTimer = 0;

    // Graph component utility

    std::vector<std::pair<std::pair<int, int>, int>> EdgeList; // store pair of id (not value as b)
    // std::map<int, CircleNode::Ptr> NodeValue; // map value to circleNode pointer
private:
    int                         findVerID(CircleNode* node);


};