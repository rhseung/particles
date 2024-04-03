#include <SFML/Graphics.hpp>
#include <event_manager.hpp>
#include <iostream>

//#include "libs/object.hpp"
//#include "libs/renderer.hpp"

int main() {
    constexpr int32_t width = 800;
    constexpr int32_t height = 600;
//    constexpr int32_t fps = 60;
//    constexpr float dt = 1.f / fps;

//    sf::ContextSettings settings;
//    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(width, height), "Space", sf::Style::Default);
//    window.setFramerateLimit(fps);

//    sfev::EventManager event_manager(window, true);
//    event_manager.addEventCallback(sf::Event::Closed, [&](const sf::Event &) { window.close(); });
//    event_manager.addKeyPressedCallback(sf::Keyboard::Escape, [&](const sf::Event &) { window.close(); });

//    pt::Renderer renderer(window);
//    auto circle1 = new pt::Circle({400.f, 300.f}, 50.f);
//    auto circle2 = new pt::Circle({250.f, 150.f}, 30.f);
//
//    renderer.addMany({circle1, circle2});

    while (window.isOpen()) {
//        event_manager.processEvents();

        window.clear(sf::Color::Black);
//        renderer.render();
        window.display();
    }

    return 0;
}
