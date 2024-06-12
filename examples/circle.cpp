#include "../Particles.hpp"

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
    solver.setSubStepsCount(8);
    solver.setSimulationUpdateRate(frame_rate);

    // Set simulation attributes
    const float object_spawn_delay = 0.025f;
    const float object_spawn_speed = 1200.0f;
    const sf::Vector2f object_spawn_position = {500.0f, 200.0f};
    const float object_min_radius = 4.0f;
    const float object_max_radius = 20.0f;
    const uint32_t max_objects_count = 300;
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
                    auto &circle = dynamic_cast<CircleBody &>(*object);
                    if (std::abs(circle.position - mouse) < 100)
                        solver.setObjectVelocity(circle, 1000.0f * Math::normalize(circle.position - mouse));
                }
            }
        }

        if (solver.getObjectsCount() < max_objects_count && clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
            clock.restart();
            Material materials[] = {
                Materials::wood, Materials::ideal, Materials::metal, Materials::ice, Materials::glass,
                Materials::stone, Materials::sand, Materials::paper, Materials::plastic, Materials::diamond,
                Materials::gold, Materials::silver, Materials::copper
            };
            auto t = solver.getTime();
            auto &object = solver.addBody(new CircleBody(object_spawn_position,
                                                           RNGf::getRange(object_min_radius, object_max_radius),
                                                           materials[solver.getObjectsCount() % 13], getRainbow(t)));
            const float angle = max_angle * std::sin(t) + Math::PI * 0.5f;
            object.velocity = object_spawn_speed * sf::Vector2f{std::cos(angle), std::sin(angle)};
        }

        solver.update();
        window.clear(sf::Color::White);
        renderer.render(solver);
        window.display();
    }

    return 0;
}
