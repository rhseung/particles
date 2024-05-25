#pragma once
#include "solver.hpp"
#include "constraints.hpp"

class Renderer {
 public:
    explicit Renderer(sf::RenderTarget &target): m_target{target} {}

    void render(const Solver &solver) const {
        // Render constraints
//        const sf::Vector3f constraint = solver.getConstraint();
//        sf::CircleShape constraint_background{constraint.z};
//        constraint_background.setOrigin(constraint.z, constraint.z);
//        constraint_background.setFillColor(sf::Color::Black);
//        constraint_background.setPosition(constraint.x, constraint.y);
//        constraint_background.setPointCount(128);
//        m_target.draw(constraint_background);
        const auto &constraints = solver.getConstraints();
        for (const auto pointer : constraints) {
            m_target.draw(*pointer->render());
        }

        // Render objects
        const auto &objects = solver.getObjects();
        for (const auto pointer : objects) {
            m_target.draw(*pointer->render());
        }
    }

 private:
    sf::RenderTarget &m_target;
};
