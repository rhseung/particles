#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "utils/math.hpp"

class Constraint {
 public:
    sf::Color color = sf::Color::Black;

    virtual void apply(Object* &obj, float dt) = 0;
    virtual sf::Drawable* render() = 0;
};

class CircleConstraint : public Constraint {
 public:
    float radius = 10.f;
    sf::Vector2f pos;
    CircleConstraint(const sf::Vector2f position, const float radius): pos{position}, radius{radius} {}

    void apply(Object* &obj, float dt) override {
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

class BoxConstraint : public Constraint {
 public:
    sf::Vector2f top_left;
    float width;
    float height;
    BoxConstraint(const sf::Vector2f top_left, const float width, const float height): top_left{top_left}, width{width}, height{height} {}

    void apply(Object* &obj, float dt) override {
        if (auto* circle = dynamic_cast<CircleObject*>(obj)) {
            const sf::Vector2f pos = circle->pos;
            const float radius = circle->radius;

            const sf::Vector2f closest = {
                std::clamp(pos.x, top_left.x, top_left.x + width),
                std::clamp(pos.y, top_left.y, top_left.y + height)
            };

            const sf::Vector2f dx = pos - closest;
            const float dist = std::abs(dx);

            // check if the circle is inside the rectangle
            // if not, move the circle to the closest point on the rectangle
            // ignore when outside the rectangle, even dist < radius
            // TODO: 밖에 달라붙는 놈들이 생김
            if (dist < radius && closest != pos) {
                const sf::Vector2f u = dx / dist;
                circle->pos = closest + u * radius;
            }
        }
    }

    sf::Drawable* render() override {
        auto* rect = new sf::RectangleShape({width, height});
        rect->setOrigin(width / 2.0f, height / 2.0f);
        rect->setPosition(top_left.x + width / 2.0f, top_left.y + height / 2.0f);
        rect->setFillColor(color);

        return rect;
    }
};

// TODO: 엄밀히는 얘는 다 튕겨내는 놈이라 Constraint라고 할 수 없음.
class WallConstraint : public Constraint {
 public:
    sf::Vector2f top_left;
    float width;
    float height;
    WallConstraint(const sf::Vector2f top_left, const float width, const float height): top_left{top_left}, width{width}, height{height} {
        color = sf::Color::White;
    }

    void apply(Object* &obj, float dt) override {
        if (auto* circle = dynamic_cast<CircleObject*>(obj)) {
            const sf::Vector2f pos = circle->pos;
            const sf::Vector2f half_size = {width * 0.5f, height * 0.5f};
            const sf::Vector2f top_right = top_left + sf::Vector2f{width, 0.f};
            const sf::Vector2f bottom_left = top_left + sf::Vector2f{0.f, height};
            const sf::Vector2f bottom_right = top_left + sf::Vector2f{width, height};

            const sf::Vector2f closest = {
                std::clamp(pos.x, top_left.x, top_right.x),
                std::clamp(pos.y, top_left.y, bottom_left.y)
            };

            const sf::Vector2f dx = pos - closest;
            const float dist = std::abs(dx);

            // Check if the circle is inside the rectangle
            // If it is, circle bounce off the rectangle and reflect the velocity using position because of verlet integration
            if (dist < circle->radius) {
                const sf::Vector2f u = dx / dist;
                circle->pos = closest + u * circle->radius;

                const sf::Vector2f v = circle->vel(dt);
                circle->set_velocity(v - 2.f * (v * u)*u, dt);
            }
        }
    }

    sf::Drawable* render() override {
        // top_left, width, height
        auto* rect = new sf::RectangleShape({width, height});
        rect->setOrigin(width * 0.5f, height * 0.5f);
        rect->setPosition(top_left + sf::Vector2f{width * 0.5f, height * 0.5f});
        rect->setFillColor(color);

        return rect;
    }
};