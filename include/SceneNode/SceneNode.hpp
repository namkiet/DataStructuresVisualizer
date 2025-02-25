#pragma once
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <vector>
#include <memory>
#include <Core/Category.hpp>

struct Command;

class SceneNode : 
    public sf::Transformable, public sf::Drawable,
    private sf::NonCopyable
{
public:
    typedef std::unique_ptr<SceneNode> Ptr;

protected:
    std::vector<Ptr>    mChildren;
    SceneNode*          mParent;

public:
                            SceneNode();
    void                    attachChild(Ptr child);
    Ptr                     detachChild(const SceneNode& node);

private:
    virtual void            draw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void            drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    void                    drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    virtual void            update(sf::Time dt);

private:
    virtual void            updateCurrent(sf::Time dt);
    void                    updateChildren(sf::Time dt);

// public:
//     sf::Vector2f		getWorldPosition() const;
//     sf::Transform		getWorldTransform() const;
public:
    virtual unsigned int    getCategory() const;

public:
    void                    executeCommand(const Command& command, sf::Time dt);

// private:
//     virtual void            addLeft(Ptr newNode) {}
};