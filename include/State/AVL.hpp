#pragma once
#include <SFML/Graphics.hpp>
#include <State/StateStack.hpp>
#include <Core/User.hpp>
#include <Core/ResourceHolder.hpp>
#include <Core/ResourceIdentifiers.hpp>
#include <SceneNode/SceneNode.hpp>
#include <SceneNode/SpriteNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <Core/CommandQueue.hpp>
#include <Core/Command.hpp>
#include <DataStructures/AVLTree.hpp>
#include <memory>
#include <array>
#include <queue>

class AVL : public State
{
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
private:
    enum Layer
    {
        Background,
        Nodes,
		Edges,
        CodeBox,
        Controller,
        OperationButotns,
        LayerCount
    };


private:
	SceneNode							mSceneGraph;
	std::array<SceneNode*, LayerCount>	mSceneLayers;
	std::vector<TreeNode*>              mNodeList;

	std::vector<int> v;
	int id;

public:
										AVL(StateStack& stack, Context context);
	virtual void						draw();
	virtual bool						update(sf::Time dt);
	virtual bool						handleEvent(const sf::Event& event);

private:
	sf::RenderWindow&					mWindow;
	TextureHolder&						mTextures;

private:
    void								loadTextures();
    void								buildScene();

public:
    CommandQueue&						getCommandQueue();

private:
    CommandQueue						mCommandQueue;

private:
	sf::Text							text;

private:
	AVLTree								mAVL;
};