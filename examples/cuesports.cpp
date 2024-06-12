#include "../Particles.hpp"

int main() {
    const Vec2 window_size{1000.f, 600.f};
    const uint32_t frame_rate = 120;
    const uint32_t antialiasing_level = 8;

    sf::RenderWindow window(sf::VideoMode(static_cast<uint32_t>(window_size.x), static_cast<uint32_t>(window_size.y)),
                            "cuesports", sf::Style::Default, sf::ContextSettings(0, 0, antialiasing_level));
    Solver solver{{0, 0}, 8, frame_rate};
    Renderer renderer{window};
    sfev::EventManager evm{window, true};

    window.setFramerateLimit(frame_rate);
    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });

    auto ball1 = new CircleBody({window_size.x / 3, window_size.y / 2}, 18.f, Materials::ideal);
    ball1->setColor(sf::Color::White);
    solver.addBody(ball1);

    sf::Color fill_colors[] = {
        sf::Color(0xffdd00ff), sf::Color(0xff3300ff),  sf::Color(0x662200ff),
        sf::Color(0xff8800ff), sf::Color(0x00bb11ff), sf::Color(0x9900ffff),sf::Color(0x0077ffff)
    };

    auto ball2 = new CircleBody({window_size.x / 2, window_size.y / 2}, 18.f, Materials::ideal);
    ball2->setColor(sf::Color::Black);
    solver.addBody(ball2);

    int cnt = -1;
    for (int i = 0; i < 5; i++) {
        float x = window_size.x / 2 + 40.f * i;
        float y = window_size.y / 2 - 20.f * i;

        for (int j = 0; j <= i; j++) {
            cnt++;
            if (i == 0 && j == 0) continue;

            auto ball = new CircleBody({x, y + 40.f * j}, 18.f, Materials::ideal);
            ball->setColor(fill_colors[cnt % 7]);
            ball->setOutlineColor(cnt % 2 == 0 ? sf::Color::White : sf::Color::Black);
            solver.addBody(ball);
        }
    }

    auto wall1 = new RectangleBody({window_size.x / 2, 0}, window_size.x + 10, 10, Materials::ideal);
    wall1->setColor(sf::Color::White);
    wall1->setOutlineColor(sf::Color::White);
    wall1->setStatic(true);
    solver.addBody(wall1);

    auto wall2 = new RectangleBody({window_size.x / 2, window_size.y}, window_size.x + 10, 10, Materials::ideal);
    wall2->setColor(sf::Color::White);
    wall2->setOutlineColor(sf::Color::White);
    wall2->setStatic(true);
    solver.addBody(wall2);

    auto wall3 = new RectangleBody({0, window_size.y / 2}, 10, window_size.y + 10, Materials::ideal);
    wall3->setColor(sf::Color::White);
    wall3->setOutlineColor(sf::Color::White);
    wall3->setStatic(true);
    solver.addBody(wall3);

    auto wall4 = new RectangleBody({window_size.x, window_size.y / 2}, 10, window_size.y + 10, Materials::ideal);
    wall4->setColor(sf::Color::White);
    wall4->setOutlineColor(sf::Color::White);
    wall4->setStatic(true);
    solver.addBody(wall4);

    bool is_dragging = false;
    Body* selected_body = nullptr;

    evm.addMousePressedCallback(sf::Mouse::Left, [&solver, &is_dragging, &selected_body](sf::Event e) {
        Vec2 pos = {static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)};
        std::for_each(solver.getBodyList().begin(), solver.getBodyList().end(), [&pos, &is_dragging, &selected_body](Body* body) {
            if (body->contains(pos)) {
                is_dragging = true;
                selected_body = body;
                return;
            }
        });
    });

    evm.addMouseReleasedCallback(sf::Mouse::Left, [&is_dragging, &selected_body](sf::Event e) {
        if (is_dragging) {
            Vec2 pos = {static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y)};
            Vec2 diff = pos - selected_body->position();

            selected_body->addForce(-diff * 4000000.f);

            is_dragging = false;
            selected_body = nullptr;
        }
    });

    renderer.addText([&solver]() {
        float energy = 0.f;
        for (auto body : solver.getBodyList()) {
            if (body->isStatic()) continue;
            energy += 0.5f * body->mass() * Math::lengthSquared(body->velocity());
        }

        return std::format("Kinetic Energy: {:.2f}", energy);
    }, sf::Color::Black);

    sf::Clock clock;
    while (window.isOpen()) {
        evm.processEvents();
        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);

        if (is_dragging) {
            Vec2 end_pos = {static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)};
            Vec2 diff = end_pos - selected_body->position();

            // line
            sf::Vertex line[] = {
                sf::Vertex(selected_body->position(), sf::Color::White),
                sf::Vertex(end_pos, sf::Color::White)
            };
            window.draw(line, 2, sf::Lines);
        }

        window.display();
    }

    return 0;
}
