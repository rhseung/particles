#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "utils/math.hpp"

class Constraint {
 public:
    sf::Color color = sf::Color::Black;

    virtual void apply(Object* &obj) = 0;
    virtual sf::Drawable* render() = 0;
};

class CircleConstraint : public Constraint {
 public:
    float radius = 10.f;
    sf::Vector2f pos;
    CircleConstraint(const sf::Vector2f position, const float radius): pos{position}, radius{radius} {}

    void apply(Object* &obj) override {
        if (auto* circle = dynamic_cast<CircleObject*>(obj)) {
            const sf::Vector2f dx = pos - circle->pos;
            const float dist = std::abs(dx);

            if (dist > (radius - circle->radius)) {
                const sf::Vector2f u = dx / dist;
                circle->pos = pos - u * (radius - circle->radius);
            }
        }
    }

    sf::Drawable* render() override {
        auto* circle = new sf::CircleShape(radius);
        circle->setPointCount(128);
        circle->setOrigin(radius, radius);
        circle->setPosition(pos);
        circle->setFillColor(color);

        return circle;
    }
};
