#include <DataStructures/Graph.hpp>
#include<vector>
#include<utility>
using namespace std;
#include <cmath>
#include <Core/Action.hpp>
Graph::Graph(){
    std::cout<<"success jump to constructor"<<std::endl;
    NumVer = 8;
    NumEdge = 13;
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

    std::cout<<"Create Graph OK"<<std::endl;
}

Graph::Graph(vector<vector<pair<int,int>>> G){

};

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
            std::cout<<"source : "<<sourceID<<"dest : "<<destID<<std::endl;
            if((*marked)[sourceID] ^ (*marked)[destID])
            {
                if(mEdgeList[j]->getColor() != sf::Color::Red)
            {
                std::cout<<"source : "<<sourceID<<"dest : "<<destID<<" inside the if statement" <<std::endl;
                if(minEdgeID == -1 || mEdgeList[j]->getWeight() < minEdge){
                    minEdgeID = j; minEdge = mEdgeList[j]->getWeight();
                }
                std::cout<<"MINEDGEID = "<<minEdgeID<<std::endl;
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
                changeEdgeColor(mEdgeList[j]->mFrom, mEdgeList[j]->mTo, VIZ::EDGE::Color, 0.5f);
            }
        }
    //reset color after perform Prim
    // mActionQueue.pushInstantAction([=](){
    //     for(auto& Ver: mNodeList){
    //         Ver->setFillColor(VIZ::NODE::FillColor);
    //     }
    // });
    std::cout<<"Jump to Prim ok"<<std::endl;


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