#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/solver.hpp"
#include "engine/renderer.hpp"
#include "engine/object.hpp"
#include "engine/constraints.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"

static sf::Color getRainbow(float t) {
    const float r = std::sin(t);
    const float g = std::sin(t + 0.33f * 2.0f * Math::PI);
    const float b = std::sin(t + 0.66f * 2.0f * Math::PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

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
    solver.addConstraint(new CircleConstraint(
        {static_cast<float>(window_width) * 0.5f, static_cast<float>(window_height) * 0.5f}, 350.0f)
    );
    // 원 중앙에 가로막는 wall
    solver.addConstraint(new WallConstraint({static_cast<float>(window_width) * 0.5f - 40.f, static_cast<float>(window_height) * 0.5f + 40.f}, 80.f, static_cast<float>(window_height)));
    solver.addConstraint(new WallConstraint({400.f, 300.f}, 200.f, 30.f));
    solver.addConstraint(new WallConstraint({400.f, 500.f}, 200.f, 30.f));

    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set simulation attributes
    const float object_spawn_delay = 0.025f;
    const float object_spawn_speed = 1200.0f;
    const sf::Vector2f object_spawn_position = {500.0f, 200.0f};
    const float object_min_radius = 4.0f;
    const float object_max_radius = 10.0f;
    const uint32_t max_objects_count = 1500;
    const float max_angle = 1.0f;

    sf::Clock clock;

    // Main loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mouse = {static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};
                for (auto &object : solver.getObjects()) {
                    auto &circle = dynamic_cast<CircleObject &>(*object);
                    if (std::abs(circle.pos - mouse) < 100)
                        solver.setObjectVelocity(circle, 1000.0f * Math::normalize(circle.pos - mouse));
                }
            }
        }

        if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            auto &object = solver.addObject(new CircleObject(object_spawn_position,
                                                       RNGf::getRange(object_min_radius, object_max_radius)));
            const float t = solver.getTime();
            const float angle = max_angle * std::sin(t) + Math::PI * 0.5f;
            solver.setObjectVelocity(object, object_spawn_speed * sf::Vector2f{std::cos(angle), std::sin(angle)});
            object.color = getRainbow(t);
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
