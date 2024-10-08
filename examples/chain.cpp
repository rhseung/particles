#include "../Particles.hpp"
#include <format>

int main() {
    const Vec2 window_size{1000.f, 800.f};
    const uint32_t frame_rate = 120;
    const uint32_t antialiasing_level = 8;

    sf::RenderWindow window(sf::VideoMode(static_cast<uint32_t>(window_size.x), static_cast<uint32_t>(window_size.y)),
                            "chain", sf::Style::Default, sf::ContextSettings(0, 0, antialiasing_level));
    Solver solver{{0, 1500}, 8, frame_rate};
    Renderer renderer{window};
    sfev::EventManager evm{window, true};

    bool run = false;

    window.setFramerateLimit(frame_rate);
    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Space, [&run](sf::Event) { run = !run; });

    auto floor = new RectangleBody({window_size.x / 2, 3 * window_size.y / 4},
                                    700.f,70.f,Materials::ideal);
    floor->setColor(sf::Color::Black);
    floor->setStatic(true);
    solver.addBody(floor);

    auto pivot = new CircleBody({window_size.x / 2, window_size.y / 2 - 200.f}, 9.f, Materials::ideal);
    pivot->setColor(getRainbow(solver.getBodyCount()));
    pivot->setStatic(true);
    solver.addBody(pivot);

    for (int i = 1; i <= 10; i++) {
        auto body = new CircleBody({window_size.x / 2 + 40.f * i, window_size.y / 2 - 200.f}, 9.f, Materials::ideal);
        body->setColor(getRainbow(solver.getBodyCount()));
        solver.addConstraint(new Chain(solver.getBody(solver.getBodyCount() - 1), body));
        solver.addBody(body);
        pivot = body;
    }

    renderer.addText([&solver]() {
        return std::format("Bodies: {}", solver.getBodyCount());
    });
    renderer.addText([&run]() {
        if (run)
            return std::pair{"State: Running", sf::Color::Green};
        else
            return std::pair{"State: Paused", sf::Color::Red};
    });

    sf::Clock clock;
    while (window.isOpen()) {
        evm.processEvents();
        if (run)
            solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
