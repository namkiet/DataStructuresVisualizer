#include <SceneNode/ProgressBar.hpp>
#include <iostream>

ProgressBar::ProgressBar(float x, float y, float width, float height) {
    bar.setPosition(x, y);
    bar.setSize({width, height});
    bar.setFillColor(sf::Color(40, 40, 40));
    bar.setOutlineThickness(1);
    bar.setOutlineColor(sf::Color(100, 100, 100));

    fill.setPosition(x, y);
    fill.setSize({0, height});
    fill.setFillColor(sf::Color(70, 180, 255));

    knob.setRadius(height * 1.5f / 2);
    knob.setFillColor(sf::Color(230, 230, 230));
    knob.setOutlineThickness(2);
    knob.setOutlineColor(sf::Color(100, 100, 100));
    knob.setOrigin(knob.getRadius(), knob.getRadius());
    knob.setPosition(x, y + height / 2);

    this->x = x;
    this->width = width;
    dragging = false;
    progress = 0.f;
}

void ProgressBar::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        if (knob.getGlobalBounds().contains(mousePos)) {
            dragging = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        dragging = false;
    } else if (event.type == sf::Event::MouseMoved && dragging) {
        float mouseX = event.mouseMove.x;
        mouseX = std::clamp(mouseX, x, x + width);
        progress = (mouseX - x) / width;
        fill.setSize({progress * width, bar.getSize().y});
        knob.setPosition(mouseX, bar.getPosition().y + bar.getSize().y / 2);
    }
}

float ProgressBar::getProgress() const {
    return progress;
}

void ProgressBar::updateCurrent(sf::Time dt) {
    // No update logic for now
}

void ProgressBar::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(bar, states);
    target.draw(fill, states);
    target.draw(knob, states);
}
