//
// Created by rhseung on 2024-03-27.
//

#pragma once

#include <SFML/Graphics.hpp>

class Object {
    sf::Vector2f x;
    sf::Vector2f x_0;
    sf::Vector2f a;
    float radius = 10.f;
    sf::Color color = sf::Color::Black;

    void update(float dt) {
        const sf::Vector2f delta_x = x - x_0;

        x_0 = x;
        x = x + delta_x + 0.5f * a * dt * dt;

        a = {};
    }

    void accelerate(sf::Vector2f acc) {
        a += acc;
    }

    void setVelocity(sf::Vector2f v, float dt) {
        x_0 = x - v * dt;
    }

    void addVelocity(sf::Vector2f v, float dt) {
        x_0 -= v * dt;
    }

public:
    Object() = default;
    Object(sf::Vector2f position, float radius): x(position), x_0(position), a{0.f, 0.f}, radius(radius) {}

    [[nodiscard]] sf::Vector2f getPosition() const {
        return x;
    }

    [[nodiscard]] sf::Vector2f getAcceleration() const {
        return a;
    }

    [[nodiscard]] sf::Color getColor() const {
        return color;
    }

    [[nodiscard]] float getRadius() const {
        return radius;
    }

    [[nodiscard]] sf::Vector2f getVelocity(float dt) const {
        return (x - x_0) / dt;
    }
};
