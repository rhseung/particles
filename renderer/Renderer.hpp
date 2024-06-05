#pragma once
#include "../physics/Solver.hpp"
#include "../engine/common/Constraints.hpp"

class Renderer {
 public:
    explicit Renderer(sf::RenderTarget &target): target{target} {}

    void render(const Solver &solver) const {
        // Render constraints
        const auto &constraints = solver.getConstraintList();
        for (const auto pointer : constraints) {
            target.draw(*pointer->render());
        }

        // Render objects
        const auto &objects = solver.getBodyList();
        for (const auto pointer : objects) {
            for (auto drawable : pointer->render()) {
                target.draw(*drawable);
            }
        }
    }

 private:
    sf::RenderTarget &target;
};
