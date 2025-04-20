#include <DataStructures/Graph.hpp>
#include<vector>
#include<utility>
#include <cmath>
#include <Core/Action.hpp>
#include <Core/Utility.hpp>
#include "Core/Variables.hpp"
#include <set>


Graph::Graph(){
    isReady = false;
    selectID = -1;

    std::cout<<"success jump to constructor"<<std::endl;
    NumVer = 8;
    NumEdge = 13;
    velocity.resize(NumVer, sf::Vector2f(0,0));
    ForceConstant = VIZ::DS::Size.x * VIZ::DS::Size.y / NumVer;

    EdgeList = {
        {{0, 1}, 4},
        {{0, 2}, 3},
        {{0, 6}, 10},
        {{0, 7}, 1},
        {{1, 3}, 2},
        {{1, 4}, 7},
        {{2, 4}, 1},
        {{2, 5}, 3},
        {{3, 5}, 5},
        {{4, 5}, 2},
        {{4, 6}, 4},
        {{5, 7}, 1},
        {{6, 7}, 6}
    };

    int n = NumVer;
    // temp NodeList
    vector<CircleNode*> ListGraphNode;
    ListGraphNode.resize(n);

    sf::Vector2f Center = VIZ::DS::Center - VIZ::DS::Position; // vi tri tuong doi

    double magnitude = 300;
    constexpr double PI = 3.14159265358979323846;

    for(int i = 0; i < n;i++){
        ListGraphNode[i] = new CircleNode(i, VIZ::NODE::Radius, sf::Color::White, sf::Color::Black);
        // set position
        float angle = 2 * PI * i / n;
        sf::Vector2f unitVec(std::cos(angle), std::sin(angle));
        ListGraphNode[i]->setPosition(Center + sf::Vector2f(unitVec.x * magnitude, unitVec.y * magnitude));
    }

    // add Edge
    for(int i = 0 ; i < EdgeList.size();i++){
        addEdge(ListGraphNode[EdgeList[i].first.first], ListGraphNode[EdgeList[i].first.second], EdgeList[i].second, false);
    }

    // add Node to the original NodeList
    for(auto& Node: ListGraphNode){
        addNode(Node);
    }

    isReady = true;

    std::cout<<"Create Graph OK"<<std::endl;
}

Graph::Graph(vector<vector<pair<int,int>>> G){

};
void Graph::handleEvent(const sf::Event& event){
    // std::cout<<"Graph can handle event"<<std::endl;
    if(event.type == sf::Event::MouseButtonPressed)
    {
        std::cout<<"Mouse position"<<event.mouseButton.x<<" "<<event.mouseButton.y<<std::endl;
        if(event.mouseButton.button == sf::Mouse::Left){
            for(int i = 0; i < mNodeList.size();i++){
                if(dist(this->getWorldPosition(i), sf::Vector2f(event.mouseButton.x, event.mouseButton.y)) < VIZ::NODE::Radius){
                    selectID = i;
                    std::cout<<"Select Node ID: "<<selectID<<std::endl;
                    break; // default set select for the first index found
                }
            }
        }
    }

    else if(event.type == sf::Event::MouseButtonReleased)
    {
        selectID = -1;
    }

    else if (event.type == sf::Event::MouseMoved) 
    {
        if (selectID != -1) {
            sf::Vector2f Pos = sf::Vector2f(event.mouseMove.x - VIZ::DS::Position.x, event.mouseMove.y - VIZ::DS::Position.y);
            if(isValidNodePosition(Pos))
            {
                changeNodePosition(selectID, Pos);
            }
            else selectID = -1;
        }
    }
}

// find edge
int Graph::EdgeID(int start, int end){ // given 2 id, not 2 value
    for(int i = 0; i < mEdgeList.size();i++){
        if(mEdgeList[i]->mFrom == mNodeList[start].get() && mEdgeList[i]->mTo == mNodeList[end].get()){
            return i;
        }
    }
    return -1;
}

int Graph::findVerID(CircleNode* node)
{
    for (int i = 0; i < mNodeList.size(); i++)
    {
        if (mNodeList[i].get() == node) {
            return i;
        }
    }
    return -1;
}

void Graph::MarkEdge(Edge* edge,int direction, float duration){
    mActionQueue.pushAction(Action::MarkEdge(edge,direction, duration));
}

void Graph::Prim()
{
    if (NumVer == 0) return;

    resetHistory();
    //     for(int i = 0; i < NumVer ;i++){
    //     std::cout<<"World Postion at"<< i<<" : "<<getWorldPosition(i).x<<" "<<getWorldPosition(i).y<<std::endl;
    // }
    auto marked = std::make_shared<std::vector<bool>>(NumVer, false);
    auto markedEdge = std::make_shared<std::vector<bool>> (mEdgeList.size(),false);

    (*marked)[0] = true;

    createNewActionGroup();
    highlightNode(mNodeList[0].get(), sf::Color::Red, 0.5f, false);
    
    int count = NumVer - 1;

    while(count--){
        createNewActionGroup();
        int minEdgeID = -1;
        int minEdge = INT_MAX;
    // 1 thang true 1 thang false thi noi lai voi nhau, neu khong thi dua ve color cu
        for(int j = 0; j < this->EdgeList.size();j++){
            int sourceID = EdgeList[j].first.first;
            int destID = EdgeList[j].first.second;
            // std::cout<<"source : "<<sourceID<<"dest : "<<destID<<std::endl;
            if((*marked)[sourceID] ^ (*marked)[destID])
            {
                if(mEdgeList[j]->getColor() != sf::Color::Blue)
            {
                // std::cout<<"source : "<<sourceID<<"dest : "<<destID<<" inside the if statement" <<std::endl;
                if(minEdgeID == -1 || EdgeList[j].second < minEdge){
                    minEdgeID = j; minEdge = EdgeList[j].second;
                }
                // std::cout<<"MINEDGEID = "<<minEdgeID<<std::endl;
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, sf::Color::Blue, 0.5f);
            }
            }
            else if(!(*markedEdge)[j]){
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, VIZ::EDGE::Color, 0.5f);
            }
        }

        if(minEdgeID == -1) break;
        
        int id1 = findVerID(mEdgeList[minEdgeID]->mFrom); 
        int id2 = findVerID(mEdgeList[minEdgeID]->mTo);
        int ToMark = (*marked)[id1]? id2 : id1;

        int direction = (ToMark == id2)? 1 : -1;  // direction to mark edge 
        (*marked)[ToMark] = true;
        (*markedEdge)[minEdgeID] = true;
        createNewActionGroup();
        highlightNode(mNodeList[ToMark].get(), sf::Color::Red, 0.5f, false);
        MarkEdge(mEdgeList[minEdgeID].get(), direction, 1.0f);
    }

    createNewActionGroup();
    for(int j = 0; j < mEdgeList.size();j++){
        if(!(*markedEdge)[j]){
            std::cout<<"original color"<<std::endl;
            changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, VIZ::EDGE::Color, 0.5f);
        }
    }
    createNewActionGroup();
    for(int j = 0; j < mEdgeList.size();j++){
        if((*markedEdge)[j]){
            std::cout<<"Highlight Edge"<<std::endl;
            traverseEdge(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, sf::Color::Green, 0.5f, false);
        }
    }

    
    createNewActionGroup();
    for (auto &node: mNodeList)
    {
        if (node) 
        {
            node->mTargetPosition = node->getPosition();
            highlightNode(node.get(), VIZ::NODE::FillColor, 0.3f, false);
        }
    }
}

void Graph::remove(int value){
    std::cout<<"Remove ok"<<std::endl;
}

void Graph::insert(int value) {
    std::cout << "Insert value: " << value << std::endl;
}

bool Graph::search(int value) {
    std::cout << "Search value: " << value << std::endl;
    return true;
}

void Graph::updateCurrent(sf::Time dt)
{
    DS::updateCurrent(dt);
    if (!isRunning())
        physicalUpdate(dt);   
}

void Graph::physicalUpdate(sf::Time dt)
{
    // attraction between adjacent node
    for(auto& edge: EdgeList){
        int start = edge.first.first;
        int end = edge.first.second;
        sf::Vector2f force = Attraction(ForceConstant, mNodeList[end]->getPosition(), mNodeList[start]->getPosition());
        velocity[start] += force * dt.asSeconds();
        velocity[end] -= force * dt.asSeconds();
    }


    // repulsion between node
    for(int i = 0 ; i < NumVer;i++)
    {
        for(int j = 0; j < NumVer;j++)
        {
            if(i == j) continue;
            sf::Vector2f force = Repulsion(ForceConstant * 10, mNodeList[j]->getPosition(), mNodeList[i]->getPosition());
            velocity[i] += force * dt.asSeconds();
        }
    }

    // center attraction
    for(int i = 0 ; i < NumVer;i++){
        sf::Vector2f force = CenterAttraction(mNodeList[i]->getPosition());
        // sf::Vector2f force = CenterAttractionNew(mNodeList[i]->getPosition(), i, NumVer);
        velocity[i] += force*dt.asSeconds();
    }

    // update position, take into account friction
    for(int i = 0 ; i < NumVer;i++)
    {
        // velocity[i] *= 0.2f; // friction
        if(i == 0) std::cout<<norm(velocity[i]) <<std::endl;
        velocity[i] = (norm(velocity[i]) < 10.f)? velocity[i] * 0.2f : velocity[i] * 0.7f;
        velocity[i] = (norm(velocity[i]) < 0.01f)? sf::Vector2f(0.f,0.f) : velocity[i];
        sf::Vector2f newPosition = mNodeList[i]->getPosition() + velocity[i] * dt.asSeconds();
        makeValidNodePosition(newPosition);
        changeNodePosition(i, newPosition);

    }
}

void Graph::empty()
{
    DS::empty();
    NumVer = NumEdge = 0;
    selectID = -1;
    velocity.clear();
    EdgeList.clear();
}

bool Graph::checkValidInput(const std::vector<int>& data)
{
    if (data.size() % 3 != 0) return false; // lỗi dữ liệu

    std::set<std::pair<int, int>> seen;

    for (size_t i = 0; i < data.size(); i += 3) {
        int u = data[i];
        int v = data[i + 1];
        // int w = data[i + 2]; // weight, không cần check ở đây

        if (u == v) return false; // self-loop

        if (u > v) std::swap(u, v); // chuẩn hóa cặp cạnh

        if (seen.count({u, v})) return false; // duplicate edge

        seen.insert({u, v});
    }

    return true;
}

bool Graph::loadFromVector(std::vector<int> numList)
{
    std::map<int,int> ValueToID; // just temporary use
    ValueToID.clear();
    if (!checkValidInput(numList)) return false;

    empty();

    std::set<int> nodeSet;
    for (int i = 0; i + 2 < numList.size(); i += 3) {
        int u = numList[i];
        int v = numList[i + 1];
        int w = numList[i + 2];
        nodeSet.insert(u);
        nodeSet.insert(v);
        EdgeList.push_back({{u, v}, w});
    }

    NumVer = nodeSet.size();
    NumEdge = EdgeList.size();

    velocity.resize(NumVer, sf::Vector2f(0,0));
    ForceConstant = 140000;

    sf::Vector2f Center = VIZ::DS::Center - VIZ::DS::Position; // vi tri tuong doi

    double magnitude = 200;
    constexpr double PI = 3.14159265358979323846;

    int n = NumVer;
    vector<CircleNode*> ListGraphNode;
    
    // for(int i = 0; i < n;i++){
    //     ListGraphNode[i] = new CircleNode(i, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor);
    //     // set position
    //     float angle = 2 * PI * i / n;
    //     sf::Vector2f unitVec(std::cos(angle), std::sin(angle));
    //     ListGraphNode[i]->setPosition(Center + sf::Vector2f(unitVec.x * magnitude, unitVec.y * magnitude));
    // }
    int curID = 0;
    for (std::set<int>::iterator it = nodeSet.begin(); it != nodeSet.end(); it++)
    {
        ListGraphNode.push_back(new CircleNode(*it, VIZ::NODE::Radius, VIZ::NODE::FillColor, VIZ::NODE::OutlineColor));
        ValueToID[*it] = curID;
        float angle = 2 * PI * curID / n;
        sf::Vector2f unitVec(std::cos(angle), std::sin(angle));
        ListGraphNode.back()->setPosition(Center + sf::Vector2f(unitVec.x * magnitude, unitVec.y * magnitude));
        curID++;
    }

    // add Node to the original NodeList
    for (auto& Node: ListGraphNode){
        addNode(Node);
    }

    //Change edgeList valueToID
    for (auto& edge : EdgeList)
    {
        edge.first.first = ValueToID[edge.first.first];
        edge.first.second = ValueToID[edge.first.second];
    }


    // add Edge
    for(int i = 0 ; i < EdgeList.size();i++){
        addEdge(ListGraphNode[EdgeList[i].first.first], ListGraphNode[EdgeList[i].first.second], EdgeList[i].second, false);
    }
    std::cout<<"return true ok"<<std::endl;
    return true;
}