#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "engine/solver.hpp"
#include "engine/renderer.hpp"
#include "engine/common/Body.hpp"
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

    Solver solver{Vec2{}};
    Renderer renderer{window};
    sfev::EventManager evm{window, true};

    solver.setSubStepsCount(4);
    solver.setSimulationUpdateRate(frame_rate);
//    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
//    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });

    constexpr float circle_radius = 20.f;
    constexpr float margin = 50.f;
    constexpr int body_count = 20;
    constexpr float speed = 100.f;

    for (int i = 0; i < body_count; ++i) {
        float x = RNGf::getRange(margin, window_width - margin);
        float y = RNGf::getRange(margin, window_height - margin);
//        solver.addBody(new CircleBody({x, y}, circle_radius, Materials::metal, getRainbow(i)));
        solver.addBody(new RectangleBody({x, y}, 2 * circle_radius, 2 * circle_radius, Materials::metal, getRainbow(i)));
    }

//    auto& obj = solver.addBody(new CircleBody({static_cast<float>(window_width) / 2, static_cast<float>(window_height) / 2}, circle_radius, Materials::metal, sf::Color::Black));
    auto& obj = solver.addBody(new RectangleBody({static_cast<float>(window_width) / 2, static_cast<float>(window_height) / 2}, 2 * circle_radius, 2 * circle_radius, Materials::metal, sf::Color::Black));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == (sf::Keyboard::W)) {
                    obj.velocity.y = -speed;
                }
                if (event.key.code == (sf::Keyboard::S)) {
                    obj.velocity.y = speed;
                }
                if (event.key.code == (sf::Keyboard::A)) {
                    obj.velocity.x = -speed;
                }
                if (event.key.code == (sf::Keyboard::D)) {
                    obj.velocity.x = speed;
                }
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == (sf::Keyboard::W)) {
                    obj.velocity.y = 0.f;
                }
                if (event.key.code == (sf::Keyboard::S)) {
                    obj.velocity.y = 0.f;
                }
                if (event.key.code == (sf::Keyboard::A)) {
                    obj.velocity.x = 0.f;
                }
                if (event.key.code == (sf::Keyboard::D)) {
                    obj.velocity.x = 0.f;
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                obj.rotate(event.mouseWheelScroll.delta * 0.1f);
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
