#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../physics/Solver.hpp"
#include "../renderer/Renderer.hpp"
#include "../engine/object.hpp"
#include "../engine/common/Constraints.hpp"
#include "../utils/number_generator.hpp"
#include "../utils/math.hpp"
#include "../utils/event_manager.hpp"

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

    auto wall1 = new RectangleBody({0.f, 0.f}, 50.f, static_cast<float>(window_height), Materials::ideal);
    wall1->setColor(sf::Color::White);
    wall1->setOutlineColor(sf::Color::White);
    wall1->setStatic(true);
    solver.addBody(wall1);

    auto wall2 = new RectangleBody({static_cast<float>(window_width) - 50.f, 0.f}, 50.f, static_cast<float>(window_height), Materials::ideal);
    wall2->setColor(sf::Color::White);
    wall2->setOutlineColor(sf::Color::White);
    wall2->setStatic(true);
    solver.addBody(wall2);

//    solver.addConstraint(new WallConstraint({0.f, 0.f}, static_cast<float>(window_width), 50.f));
//    solver.addConstraint(new WallConstraint({0.f, static_cast<float>(window_height) - 50.f}, static_cast<float>(window_width), 50.f));

    auto wall3 = new RectangleBody({0.f, 0.f}, static_cast<float>(window_width), 50.f, Materials::ideal);
    wall3->setColor(sf::Color::White);
    wall3->setOutlineColor(sf::Color::White);
    wall3->setStatic(true);
    solver.addBody(wall3);

    auto wall4 = new RectangleBody({0.f, static_cast<float>(window_height) - 50.f}, static_cast<float>(window_width), 50.f, Materials::ideal);
    wall4->setColor(sf::Color::White);
    wall4->setOutlineColor(sf::Color::White);
    wall4->setStatic(true);
    solver.addBody(wall4);

    solver.setSubSteps(8);
    solver.setUpdateRate(frame_rate);

//    auto &circle = solver.addObject(new CircleObject({100.f, 100.f}, 20.f));
    auto rect1 = new RectangleBody({200.f, 100.f}, 30.f, 30.f, Materials::ideal);
    rect1->setColor(sf::Color::Red);
    solver.addBody(rect1);

    auto rect2 = new RectangleBody({300.f, 100.f}, 30.f, 30.f, Materials::ideal);
    rect2->setColor(sf::Color::Blue);
    solver.addBody(rect2);

    const float acc = 10000.f;
    const float jump = acc * 100;
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
                rect1->accelerate({0.f, -jump});
                isJump1 = true;
                // 점프키 뗄 때까지 기다림
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && isJump1) {
                isJump1 = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                rect1->accelerate({-acc, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                rect1->accelerate({acc, 0.f});
            }

            // Move circle2 using arrow keys
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !isJump2) {
                rect2->accelerate({0.f, -jump});
                isJump2 = true;
            }
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && isJump2) {
                isJump2 = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                rect2->accelerate({-acc, 0.f});
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                rect2->accelerate({acc, 0.f});
            }
        }

        solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
