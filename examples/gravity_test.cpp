#include "../Particles.hpp"

int main() {
    const Vec2 window_size{1000.f, 800.f};
    const uint32_t frame_rate = 120;
    const uint32_t antialiasing_level = 8;

    sf::RenderWindow window(sf::VideoMode(static_cast<uint32_t>(window_size.x), static_cast<uint32_t>(window_size.y)),
                            "gravity_test", sf::Style::Default, sf::ContextSettings(0, 0, antialiasing_level));
    Solver solver{{0.0f, 1500.0f}, 16, frame_rate};
    Renderer renderer{window};
    sfev::EventManager evm{window, true};

    window.setFramerateLimit(frame_rate);
    evm.addEventCallback(sf::Event::Closed, [&window](sf::Event) { window.close(); });
    evm.addKeyPressedCallback(sf::Keyboard::Escape, [&window](sf::Event) { window.close(); });

    // TODO: 좌표축 원점을 현실과 동일하게 아래 왼쪽으로 두기
    // TODO: 길이와 값을 전부 현실에 동기화 시키기 e.g. 중력은 9.81로.

    auto &floor = solver.addBody(new RectangleBody({window_size.x / 2, 3 * window_size.y / 4},
                                                   700.f,
                                                   70.f,
                                                   Materials::rubber));
    floor.setColor(sf::Color::Black).setStatic(true);

    evm.addMousePressedCallback(sf::Mouse::Left, [&](sf::Event event) {
//        if (solver.getBodyCount() % 2 == 0)
//            solver.addBody(new RectangleBody({(float) event.mouseButton.x, (float) event.mouseButton.y},
//                                             RNGf::getRange(20.f, 35.f),
//                                             RNGf::getRange(20.f, 35.f),
//                                             Materials::rubber)).setColor(getRainbow(solver.getTime()));
//        else
        solver.addBody(new CircleBody({(float) event.mouseButton.x, (float) event.mouseButton.y},
                                      RNGf::getRange(10.f, 20.f),
                                      Materials::rubber)).setColor(getRainbow(solver.getTime()));

        std::cout << solver.getBodyCount() << std::endl;
    });

    sf::Clock clock;
    while (window.isOpen()) {
        for (auto &obj : solver.getBodyList()) {
            if ((obj->position().x < 0 || obj->position().x > window_size.x) || (obj->position().y > window_size.y)) {
                solver.removeBody(obj);
                std::cout << solver.getBodyCount() << std::endl;
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
