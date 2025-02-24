#include <SceneNode.hpp>
#include <Command.hpp>

SceneNode::SceneNode()
{
}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
        [&node](Ptr& child) { return child.get() == &node; });

    if (found == mChildren.end())
        throw std::runtime_error("Node not found");

    Ptr result = std::move(*found); 
    mChildren.erase(found);
    result->mParent = nullptr;
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

// sf::Vector2f SceneNode::getWorldPosition() const
// {
// 	return getWorldTransform() * sf::Vector2f();
// }

// sf::Transform SceneNode::getWorldTransform() const
// {
// 	sf::Transform transform = sf::Transform::Identity;

// 	for (const SceneNode* node = this; node != nullptr; node = node->mParent)
// 		transform = node->getTransform() * transform;

// 	return transform;
// }

unsigned int SceneNode::getCategory() const
{
    return Category::Scene;
}

void SceneNode::executeCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory())
        command.action(*this, dt);
    
    for (Ptr& child: mChildren)
        child->executeCommand(command, dt);
}