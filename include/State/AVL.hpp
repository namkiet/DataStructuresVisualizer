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
#include <array>
#include <queue>

class AVL : public State
{	
public:
	struct Node
	{
		int value;
		Node *left, *right, *parent;
		Node(int value): value(value), left(nullptr), right(nullptr) {};
	};
	
private:
	Node*								mRoot;
	void 								insert(AVL::Node* &node, int value);

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
	TreeNode*							mRootNode;

private:
	void								insert(int value);
};