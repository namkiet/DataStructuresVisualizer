#pragma once
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <DataStructures/DS.hpp>

class HeapTree: public DS
{
public:
                                HeapTree();
    void                        insert(int value) override;
    void                        remove(int value) override;
    bool                        search(int value) override;
    void                        empty() override;

private:
    TreeNode*                   mRoot;

private:
    void                        align() override { align(0); };
    void                        align(int index, sf::Vector2f curPos = sf::Vector2f(VIZ::DS::Size.x / 2, VIZ::DS::RowSpacing), float curSpacingX = VIZ::DS::Size.x / 4, float curSpacingY = VIZ::DS::RowSpacing); 
    void                        heapifyUp(int index); 
    void                        heapifyDown(int index);
    
private:
    void                        saveState(std::stack<History> &stack) override;
    void                        loadState(History history) override;
};