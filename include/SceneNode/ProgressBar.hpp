#pragma once
#include <SFML/Graphics.hpp>
#include <SceneNode/SceneNode.hpp>
#include <memory>

class ProgressBar : public SceneNode {
public:
    typedef std::unique_ptr<ProgressBar> Ptr;
    ProgressBar(float x, float y, float width, float height);

    bool handleEvent(const sf::Event& event);
    float getProgress() const;
    void setProgress(float p);

private:
    virtual void updateCurrent(sf::Time dt) override;
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape bar;
    sf::RectangleShape fill;
    sf::CircleShape knob;
    float x, width;
    float progress;
    bool dragging;
};
