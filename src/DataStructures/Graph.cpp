#include <DataStructures/Graph.hpp>
#include<vector>
#include<utility>
using namespace std;
#include <cmath>
#include <Core/Action.hpp>
#include <Core/Utility.hpp>
Graph::Graph(){
    isReady = false;
    selectID = -1;

    std::cout<<"success jump to constructor"<<std::endl;
    NumVer = 8;
    NumEdge = 13;
    
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

    sf::Vector2f Center = sf::Vector2f(450.f,350.f);

    double magnitude = 200;
    constexpr double PI = 3.14159265358979323846;

    for(int i = 0; i < n;i++){
        ListGraphNode[i] = new CircleNode(i, 16.f, sf::Color::White, sf::Color::Black);
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
            changeNodePosition(selectID, Pos);
        }
    }
}

// find edge
int Graph::EdgeID(int start, int end){
    for(int i = 0; i < mEdgeList.size();i++){
        if(mEdgeList[i]->mFrom->mValue == start && mEdgeList[i]->mTo->mValue == end){
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
    //     for(int i = 0; i < NumVer ;i++){
    //     std::cout<<"World Postion at"<< i<<" : "<<getWorldPosition(i).x<<" "<<getWorldPosition(i).y<<std::endl;
    // }
    auto marked = std::make_shared<std::vector<bool>>(NumVer, false);
    auto markedEdge = std::make_shared<std::vector<bool>> (mEdgeList.size(),false);

    (*marked)[0] = true;

    createNewActionGroup();
    changeNodeColor(mNodeList[0].get(), sf::Color::Green,1.0f);
    
    int count = NumVer - 1;

    while(count--){
        createNewActionGroup();
        int minEdgeID = -1;
        int minEdge = INT_MAX;
    // 1 thang true 1 thang false thi noi lai voi nhau, neu khong thi dua ve color cu
        for(int j = 0; j < mEdgeList.size();j++){
            int sourceID = mEdgeList[j]->mFrom->mValue;
            int destID = mEdgeList[j]->mTo->mValue;
            // std::cout<<"source : "<<sourceID<<"dest : "<<destID<<std::endl;
            if((*marked)[sourceID] ^ (*marked)[destID])
            {
                if(mEdgeList[j]->getColor() != sf::Color::Red)
            {
                // std::cout<<"source : "<<sourceID<<"dest : "<<destID<<" inside the if statement" <<std::endl;
                if(minEdgeID == -1 || mEdgeList[j]->getWeight() < minEdge){
                    minEdgeID = j; minEdge = mEdgeList[j]->getWeight();
                }
                // std::cout<<"MINEDGEID = "<<minEdgeID<<std::endl;
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, sf::Color::Red, 0.5f);
            }
            }
            else if(!(*markedEdge)[j]){
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, VIZ::EDGE::Color, 0.5f);
            }
        }

        if(minEdgeID == -1) break;

        int ToMark = (*marked)[mEdgeList[minEdgeID]->mFrom->mValue]? mEdgeList[minEdgeID]->mTo->mValue : mEdgeList[minEdgeID]->mFrom->mValue;

        int direction = (ToMark == mEdgeList[minEdgeID]->mTo->mValue)? 1 : -1;  // direction to mark edge 
        (*marked)[ToMark] = true;
        (*markedEdge)[minEdgeID] = true;
        createNewActionGroup();
        changeNodeColor(mNodeList[ToMark].get(), sf::Color::Green, 0.5f);
        // changeEdgeColor(mEdgeList[minEdgeID]->mFrom, mEdgeList[minEdgeID]->mTo, sf::Color::Blue, 0.5f);
        MarkEdge(mEdgeList[minEdgeID].get(), direction, 1.0f);
    }

        createNewActionGroup();
        for(int j = 0; j < mEdgeList.size();j++){
            if(!(*markedEdge)[j]){
                std::cout<<"original color"<<std::endl;
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, sf::Color(187, 195, 204), 0.5f);
            }
        }
        createNewActionGroup();
        for(int j = 0; j < mEdgeList.size();j++){
            if((*markedEdge)[j]){
                std::cout<<"Highlight Edge"<<std::endl;
                traverseEdge(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, sf::Color::Green, 5.0f);
            }
        }

    //reset color after perform Prim
    // mActionQueue.pushInstantAction([=](){
    //     for(auto& Ver: mNodeList){
    //         Ver->setFillColor(VIZ::NODE::FillColor);
    //     }
    // });
    std::cout<<"Prim ok"<<std::endl;


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
void Graph::saveState() {
    
}

void Graph::loadState(History history) {

}

// void Graph::updateCurrent(sf::Time dt)
// {

//     if(!isReady || mNodeList.empty())
//     {
//         if(!isReady)std::cout<<"Not ready"<<std::endl;
//         else std::cout<<"NodeList is empty"<<std::endl;
//     }
//     velocity.resize(NumVer,(sf::Vector2f(0,0)));
//     std::cout<<"update graph"<<std::endl;
//     // // change position based on force

//     // // for(int i = 0 ; i < NumVer;i++){
//     // //     std::cout<<mNodeList[i]->mValue<<std::endl;
//     // // }

//     for(auto& edge: EdgeList){
//         int start = edge.first.first;
//         int end = edge.first.second;
//         mActionQueue.pushInstantAction([=](){
//             velocity[start] += Attraction(ForceConstant, mNodeList[end]->getPosition(), mNodeList[start]->getPosition());
//             velocity[end] += Attraction(ForceConstant, mNodeList[start]->getPosition(), mNodeList[end]->getPosition());
//         });

//     }
//     for(int i = 0 ; i < NumVer;i++)
//     {
//         for(int j = 0; j < NumVer;j++)
//         {
//             if(i == j) continue;
//             mActionQueue.pushInstantAction([=](){
//                 velocity[i] += Repulsion(ForceConstant, mNodeList[j]->getPosition(), mNodeList[i]->getPosition());
//             });
//         }
//     }

//     for(int i = 0 ; i < NumVer;i++)
//     {
//         mActionQueue.pushInstantAction([=](){
//                 changeNodePosition(i, mNodeList[i]->getPosition() + velocity[i] * dt.asSeconds());
//         });

//     }
//         mActionQueue.update(dt);

//     for (auto &edge: mEdgeList)
//         if (edge) edge->update(dt);

//     for (auto &node: mNodeList)
//         if (node) node->update(dt);


// }