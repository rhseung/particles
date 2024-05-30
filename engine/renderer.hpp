#pragma once
#include "solver.hpp"
#include "constraints.hpp"

class Renderer {
 public:
    explicit Renderer(sf::RenderTarget &target): m_target{target} {}

    void render(const Solver &solver) const {
        // Render constraints
        const auto &constraints = solver.getConstraints();
        for (const auto pointer : constraints) {
            m_target.draw(*pointer->render());
        }

        // Render objects
        const auto &objects = solver.getObjects();
        for (const auto pointer : objects) {
            for (auto drawable : pointer->render()) {
                m_target.draw(*drawable);
            }
        }
    }

 private:
    sf::RenderTarget &m_target;
};
