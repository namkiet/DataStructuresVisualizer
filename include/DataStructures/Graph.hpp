#pragma once
#include<vector>
#include<utility>
using namespace std;
#include <DataStructures/DS.hpp>

class Graph: public DS
{
    public:
                        Graph(vector<vector<pair<int,int>>> G);  

                        // to debug
                        Graph();
            void        Prim();
            void                        insert(int value) override;
            void                        remove(int value) override;
            bool                        search(int value) override;
            void                        saveState() override;
            void                        loadState(History history) override;

            int                         EdgeID(int start, int end);

            void                        MarkEdge(Edge* edge, int direction, float duration); // direction = 1 if edge from mFrom to mTo, otherwise -1
    

    private:
            int NumVer;
            int NumEdge;
            std::vector<std::pair<std::pair<int, int>, int>> EdgeList;
            
            


};