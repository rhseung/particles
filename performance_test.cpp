#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/solver.hpp"
#include "engine/renderer.hpp"
#include "engine/object.hpp"
#include "engine/constraints.hpp"
#include "utils/number_generator.hpp"
#include "utils/math.hpp"
#include "utils/event_manager.hpp"

static sf::Color getRainbow(float t) {
    const float r = std::sin(t);
    const float g = std::sin(t + 0.33f * 2.0f * Math::PI);
    const float b = std::sin(t + 0.66f * 2.0f * Math::PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

int main() {
    constexpr int window_width = 1000;
    constexpr int window_height = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Verlet", sf::Style::Default, settings);
    const uint32_t frame_rate = 120;
    window.setFramerateLimit(frame_rate);

    Solver solver;
    Renderer renderer{window};
    sfev::EventManager evm(window, true);

    bool emit = true;

    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Space, [&emit](sf::Event) { emit = !emit; });

    solver.addConstraint(new WallConstraint({-15.f, 0.f}, 20.f, static_cast<float>(window_height)));
    solver.addConstraint(new WallConstraint({static_cast<float>(window_width) - 5.f, 0.f},
                                            20.f,
                                            static_cast<float>(window_height)));
    solver.addConstraint(new WallConstraint({0.f, -15.f}, static_cast<float>(window_width), 20.f));
    solver.addConstraint(new WallConstraint({0.f, static_cast<float>(window_height) - 5.f},
                                            static_cast<float>(window_width),
                                            20.f));
    solver.setSubStepsCount(4);
    solver.setSimulationUpdateRate(frame_rate);

    const float object_spawn_delay = 0.025f;

    sf::Clock clock;
    while (window.isOpen()) {
        if (emit) {
            for (int i = 20; i--;) {
                const sf::Vector2f origin = {0.f, 100.f};
                const sf::Vector2f position = {100.f, 80.f + 2.f * i};
                auto &obj = solver.addObject(new CircleObject(position, 2.f));
                const float t = solver.getTime();
                solver.setObjectVelocity(obj, position - origin);
                obj.color = getRainbow(t);
            }
        }

        evm.processEvents();
        solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
