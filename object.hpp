//
// Created by rhseung on 2024-03-27.
//

#pragma once

#include <SFML/Graphics.hpp>

namespace pt {
class Object {
 public:
    sf::Vector2f pos;
    sf::Vector2f pos_0;
    sf::Vector2f acc;
    sf::Color color = sf::Color::White;

    explicit Object(const sf::Vector2f position): pos{position}, pos_0{position}, acc{0.f, 0.f} {}

    void update(const float dt) {
        const sf::Vector2f delta_x = pos - pos_0;

        pos_0 = pos;
        pos = pos + delta_x + 0.5f * acc * dt * dt;
        acc = {};
    }

    void add_acc(const sf::Vector2f a) {
        acc += a;
    }

    void set_vel(const sf::Vector2f v, const float dt) {
        pos_0 = pos - v * dt;
    }

    void add_vel(const sf::Vector2f v, const float dt) {
        pos_0 -= v * dt;
    }

    [[nodiscard]] sf::Vector2f vel(const float dt) const {
        return (pos - pos_0) / dt;
    }

    virtual sf::Drawable* render() = 0;
};

class Circle : public Object {
 public:
    float radius = 10.f;

    Circle(const sf::Vector2f position, const float radius): Object(position), radius{radius} {}

    sf::Drawable* render() override {
        auto* circle = new sf::CircleShape(radius);
        circle->setPosition(pos);
        circle->setFillColor(color);

        return circle;
    }
};
}