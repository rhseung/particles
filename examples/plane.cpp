#include "../Particles.hpp"

int main() {
    constexpr int window_width = 1000;
    constexpr int window_height = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "plane", sf::Style::Default, settings);
    const uint32_t frame_rate = 120;
    window.setFramerateLimit(frame_rate);

    Solver solver{Vec2{}};
    Renderer renderer{window};
    sfev::EventManager evm{window, true};

    solver.setSubSteps(8);
    solver.setUpdateRate(frame_rate);
//    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
//    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });

    constexpr float circle_radius = 20.f;
    constexpr float margin = 50.f;
    constexpr int body_count = 50;
    constexpr float force_magnitude = 5000000.f;

    for (int i = 0; i < body_count; ++i) {
        float x = RNGf::getRange(margin, window_width - margin);
        float y = RNGf::getRange(margin, window_height - margin);
        if (i % 6 == 0)
            solver.addBody(new CircleBody({x, y}, circle_radius, Materials::rubber, getRainbow(i)));
        else
            solver.addBody(new RegularPolygonBody({x, y}, circle_radius, i % 6 + 2, Materials::rubber, getRainbow(i)));
    }

    auto& obj = solver.addBody(new CircleBody({static_cast<float>(window_width) / 2, static_cast<float>(window_height) / 2}, circle_radius, Materials::rubber, sf::Color::Black));
//    auto& obj = solver.addBody(new RectangleBody({static_cast<float>(window_width) / 2, static_cast<float>(window_height) / 2},2 * circle_radius,2 * circle_radius,Materials::rubber,sf::Color::Black));
    obj.setMaxSpeed(150.f);

    auto& rect = solver.addBody(new RectangleBody({static_cast<float>(window_width) / 2, 500.f}, 500.f, 100.f, Materials::rubber, sf::Color::Black, true));

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                obj.rotate(event.mouseWheelScroll.delta * 0.1f);
            }
        }

        float dx = 0.f;
        float dy = 0.f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dy--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dy++;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dx--;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dx++;

        auto force = force_magnitude * Math::normalize(Vec2{dx, dy});
        obj.applyForce(force);

        evm.processEvents();
        solver.update();
        window.clear(sf::Color::Black);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
