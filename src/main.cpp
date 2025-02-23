#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2.6.2 Test");

    sf::RectangleShape rectangle(sf::Vector2f(200, 150));
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setPosition(300, 225);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(rectangle);
        window.display();
    }

    return 0;
}
