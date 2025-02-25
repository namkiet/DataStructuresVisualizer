#include <Core/User.hpp>
#include <SceneNode/TreeNode.hpp>
#include <SceneNode/TreeNode.hpp>
#include <iostream>

struct NodeMover
{
    float x, y;
    NodeMover(float x, float y): x(x), y(y) {};

    void operator() (TreeNode& node, sf::Time) const
    {
        node.move(sf::Vector2f(x, y));
    }
};

void addLeftNode(TreeNode& node, sf::Time dt)
{
    // std::unique_ptr<TreeNode> newObj = std::make_unique<TreeNode>(TreeNode::Node, 10, 20.f, sf::Color::White, sf::Color::Black);
	// newObj->setPosition(-100, 100);
	// node.attachChild(std::move(newObj));
    // std::cerr << "HI \n";
    node.move(sf::Vector2f(0, 2000.f) * dt.asSeconds());
}

// void addRightNode(TreeNode& node, sf::Time dt)
// {
//     std::unique_ptr<TreeNode> newObj = std::make_unique<TreeNode>(TreeNode::Node, 10, 20.f, sf::Color::White, sf::Color::Black);
// 	newObj->setPosition(100, 100);
// 	node.attachChild(std::move(newObj));
//     // node.addLeft(std::move(newObj));
// }

void User::handleEvent(sf::Event event, CommandQueue& commands)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::A)
        {
            Command c;
            c.category = Category::TreeNode;
            c.action = derivedAction<TreeNode>(addLeftNode);
            commands.push(c);
        }
    }
}