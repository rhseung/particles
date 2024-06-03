#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/solver.hpp"
#include "engine/renderer.hpp"
#include "engine/object.hpp"
#include "engine/constraints.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"

int main() {
    // Create window
    constexpr int window_width = 1000;
    constexpr int window_height = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Verlet", sf::Style::Default, settings);
    const uint32_t frame_rate = 120;
    window.setFramerateLimit(frame_rate);

    Solver solver;
    Renderer renderer{window};

    // Solver configuration
    solver.addConstraint(new WallConstraint({0.f, 0.f}, 50.f, static_cast<float>(window_height)));
    solver.addConstraint(new WallConstraint({static_cast<float>(window_width) - 50.f, 0.f}, 50.f, static_cast<float>(window_height)));
    solver.addConstraint(new WallConstraint({0.f, 0.f}, static_cast<float>(window_width), 50.f));
    solver.addConstraint(new WallConstraint({0.f, static_cast<float>(window_height) - 50.f}, static_cast<float>(window_width), 50.f));
    solver.addConstraint(new CircleConstraint(
        {static_cast<float>(window_width) * 0.5f, static_cast<float>(window_height) * 0.5f}, 350.0f)
    );
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set Two circles
    auto &circle1 = solver.addObject(new CircleObject({300.0f, 200.0f}, 30.0f));
    circle1.color = sf::Color::Red;
    auto &circle2 = solver.addObject(new CircleObject({700.0f, 200.0f}, 20.0f));
    circle2.color = sf::Color::Blue;

    const float acc = 10000.f;
    const float jump = acc * 150;
    bool isJump1 = false;
    bool isJump2 = false;

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            // Move circle1 using WASD
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !isJump1) {
                circle1.accelerate({0.f, -jump});
                isJump1 = true;
                // 점프키 뗄 때까지 기다림
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isJump1) {
                isJump1 = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                circle1.accelerate({-acc, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                circle1.accelerate({acc, 0.f});
            }

            // Move circle2 using arrow keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJump2) {
                circle2.accelerate({0.f, -jump});
                isJump2 = true;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isJump2) {
                isJump2 = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                circle2.accelerate({-acc, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                circle2.accelerate({acc, 0.f});
            }
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
