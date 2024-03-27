#include <SFML/Graphics.hpp>
#include <iostream>

#include "Object.hpp"

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    Object obj({400.f, 300.f}, 50.f);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        window.clear(sf::Color::White);

        sf::CircleShape circle(obj.getRadius());
        circle.setPosition(obj.getPosition());
        circle.setFillColor(obj.getColor());

        window.draw(circle);

        window.display();
    }

    return 0;
}
