//
// Created by rhseung on 2024-03-27.
//

#pragma once

#include <SFML/Graphics.hpp>

namespace pt {
    // template <typename T> requires std::is_base_of_v<sf::Drawable, T>
    class Object {
    public:
        virtual ~Object() = default;

        sf::Vector2f pos;
        sf::Vector2f pos_0;
        sf::Vector2f acc;
        sf::Color color = sf::Color::White;

        void create(const sf::Vector2f position) {
            pos = position;
            pos_0 = position;
            acc = {0.f, 0.f};
        }

        void update(const float dt) {
            const sf::Vector2f delta_x = pos - pos_0;

            pos_0 = pos;
            pos = pos + delta_x + 0.5f * acc * dt * dt;

            acc = {};
        }

        void accelerate(const sf::Vector2f a) {
            acc += a;
        }

        void setVelocity(const sf::Vector2f v, const float dt) {
            pos_0 = pos - v * dt;
        }

        void addVelocity(const sf::Vector2f v, const float dt) {
            pos_0 -= v * dt;
        }

        [[nodiscard]] sf::Vector2f getVelocity(const float dt) const {
            return (pos - pos_0) / dt;
        }

        // abstract method 'draw'
        sf::CircleShape draw();
    };

    class Circle : public Object {
    public:
        float radius = 10.f;

        Circle(const sf::Vector2f position, const float radius) {
            create(position);
            this->radius = radius;
        }

        sf::CircleShape draw() {
            sf::CircleShape circle(radius);
            circle.setPosition(pos);
            circle.setFillColor(color);

            return circle;
        }
    };
}