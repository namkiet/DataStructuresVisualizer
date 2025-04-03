#include <SceneNode/SceneNode.hpp>
#include <Core/Command.hpp>
#include <iostream>

SceneNode::SceneNode()
{
}

void SceneNode::attachChild(Ptr child)
{
    child->mPar = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
        [&node](Ptr& child) { return child.get() == &node; });

    if (found == mChildren.end())
        throw std::runtime_error("Node not found");

    Ptr result = std::move(*found); 
    mChildren.erase(found);
    result->mPar = nullptr;
    return result;
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
    // Do nothing
}

void SceneNode::updateChildren(sf::Time dt)
{
    for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        (*it)->update(dt);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Do nothing
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto it = mChildren.begin(); it != mChildren.end(); it++)
        (*it)->draw(target, states);
}


unsigned int SceneNode::getCategory() const
{
    return Category::Scene;
}

void SceneNode::executeCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory())
        command.action(*this, dt);
    
    for (auto &child: mChildren)
        child->executeCommand(command, dt);
}