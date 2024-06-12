#pragma once

#include <SFML/Graphics.hpp>
#include "Body.hpp"
#include "../../utils/math.hpp"
#include "Renderable.hpp"

class Constraint : public Renderable {
 public:
    sf::Color color = sf::Color::White;

    virtual void apply() = 0;
};

class Chain : public Constraint {
 public:
    Body* body_1;
    Body* body_2;
    float target_dist;

    Chain(Body* body_1, Body* body_2, float dist): body_1{body_1}, body_2{body_2}, target_dist{dist} {}

    void apply() override {
        Vec2 p1 = body_1->position();
        Vec2 p2 = body_2->position();

        Vec2 d = p1 - p2;
        float dist = std::abs(d);
        Vec2 n = d / dist;
//        std::cout << "normal of " << body_1->isStatic() << "," << body_2->isStatic() << ": " << n << std::endl;

        float delta = target_dist - dist;
        float obj1_ratio = body_2->isStatic() ? 1.f : 1.f / (1.f + body_1->mass() / body_2->mass());
        float obj2_ratio = body_1->isStatic() ? 1.f : 1.f / (1.f + body_2->mass() / body_1->mass());

        body_1->move(obj1_ratio * delta * n);
        body_2->move(-obj2_ratio * delta * n);
    }

    std::vector<sf::Drawable*> render() override {
        // 흰 선
        auto* line = new sf::VertexArray(sf::Lines, 2);
        line->operator[](0).position = body_1->position();
        line->operator[](1).position = body_2->position();
        line->operator[](0).color = color;
        line->operator[](1).color = color;

        return {line};
    }
};

//class CircleConstraint : public Constraint {
// public:
//    float radius = 10.f;
//    sf::Vector2f pos;
//    CircleConstraint(const sf::Vector2f position, const float radius): pos{position}, radius{radius} {}
//
//    void apply(Body* &obj, float dt) override {
//        if (obj->shape() == ShapeType::CIRCLE) {
//            auto circle = dynamic_cast<CircleBody*>(obj);
//
//            const sf::Vector2f dx = pos - circle->position();
//            const float dist = std::abs(dx);
//            if (dist > (radius - circle->radius())) {
//                const sf::Vector2f u = dx / dist;
//                circle->moveTo(pos - u * (radius - circle->radius()));
//            }
//        }
//    }
//
//    sf::Drawable* render() override {
//        auto* circle = new sf::CircleShape(radius);
//        circle->setPointCount(128);
//        circle->setOrigin(radius, radius);
//        circle->setPosition(pos);
//        circle->setFillColor(color);
//
//        return circle;
//    }
//};

//class BoxConstraint : public Constraint {
// public:
//    sf::Vector2f top_left;
//    float m_width;
//    float m_height;
//    BoxConstraint(const sf::Vector2f top_left, const float m_width, const float m_height): top_left{top_left}, m_width{m_width}, m_height{m_height} {}
//
//    void apply(Body* &obj, float dt) override {
//        if (auto* circle = dynamic_cast<CircleObject*>(obj)) {
//            const sf::Vector2f pos = circle->position;
//            const float radius = circle->radius;
//
//            const sf::Vector2f closest = {
//                std::clamp(pos.x, top_left.x, top_left.x + m_width),
//                std::clamp(pos.y, top_left.y, top_left.y + m_height)
//            };
//
//            const sf::Vector2f dx = pos - closest;
//            const float target_dist = std::abs(dx);
//
//            // check if the circle is inside the rectangle
//            // if not, move the circle to the closest point on the rectangle
//            // ignore when outside the rectangle, even target_dist < radius
//            // TODO: 밖에 달라붙는 놈들이 생김
//            if (target_dist < radius && closest != pos) {
//                const sf::Vector2f u = Math::normalize(dx);
//                circle->position = closest + u * radius;
//            }
//        }
//    }
//
//    sf::Drawable* render() override {
//        auto* rect = new sf::RectangleShape({m_width, m_height});
//        rect->setOrigin(m_width / 2.0f, m_height / 2.0f);
//        rect->setPosition(top_left.x + m_width / 2.0f, top_left.y + m_height / 2.0f);
//        rect->setFillColor(color);
//
//        return rect;
//    }
//};

// TODO: 엄밀히는 얘는 다 튕겨내는 놈이라 Constraint라고 할 수 없음.
//class WallConstraint : public Constraint {
// public:
//    sf::Vector2f top_left;
//    float m_width;
//    float m_height;
//    WallConstraint(const sf::Vector2f top_left, const float m_width, const float m_height): top_left{top_left}, m_width{m_width}, m_height{m_height} {
//        color = sf::Color::White;
//    }
//
//    void apply(Body* &obj, float dt) override {
//        if (auto* circle = dynamic_cast<CircleObject*>(obj)) {
//            const sf::Vector2f pos = circle->position;
//            const sf::Vector2f half_size = {m_width * 0.5f, m_height * 0.5f};
//            const sf::Vector2f top_right = top_left + sf::Vector2f{m_width, 0.f};
//            const sf::Vector2f bottom_left = top_left + sf::Vector2f{0.f, m_height};
//            const sf::Vector2f bottom_right = top_left + sf::Vector2f{m_width, m_height};
//
//            const sf::Vector2f closest = {
//                std::clamp(pos.x, top_left.x, top_right.x),
//                std::clamp(pos.y, top_left.y, bottom_left.y)
//            };
//
//            const sf::Vector2f dx = pos - closest;
//            const float target_dist = std::abs(dx);
//
//            // Check if the circle is inside the rectangle
//            // If it is, circle bounce off the rectangle and reflect the velocity using position because of verlet integration
//            if (target_dist < circle->radius) {
//                const sf::Vector2f u = Math::normalize(dx);
//                circle->position = closest + u * circle->radius;
//
//                // TODO: 반발계수 설정 근데 반발계수는 벽이 아니라 물체가 결정하는 것.
//                const sf::Vector2f v = circle->vel(dt);
//                circle->set_velocity(v - 2.f * (v*u) * u, dt);  // TODO: 테스트
//            }
//
//            // TODO: 벽 안으로 들어가면 무조건 뛰쳐나오게
//        }
//        else if (auto* rect = dynamic_cast<RectangleObject*>(obj)) {
//            const sf::Vector2f pos = rect->position;
//            const sf::Vector2f half_size = {m_width * 0.5f, m_height * 0.5f};
//            const sf::Vector2f top_right = top_left + sf::Vector2f{m_width, 0.f};
//            const sf::Vector2f bottom_left = top_left + sf::Vector2f{0.f, m_height};
//            const sf::Vector2f bottom_right = top_left + sf::Vector2f{m_width, m_height};
//
//            const sf::Vector2f closest = {
//                std::clamp(pos.x, top_left.x, top_right.x),
//                std::clamp(pos.y, top_left.y, bottom_left.y)
//            };
//
//            const sf::Vector2f dx = pos - closest;
//            const float target_dist = std::abs(dx);
//
//            // Check if the circle is inside the rectangle
//            // If it is, circle bounce off the rectangle and reflect the velocity using position because of verlet integration
//            if (target_dist < rect->m_width * 0.5f) {
//                const sf::Vector2f u = Math::normalize(dx);
//                rect->position = closest + u * (rect->m_width * 0.5f);
//
//                // 튕겨나오게
//                const sf::Vector2f v = rect->vel(dt);
//                rect->set_velocity(v - 2.f * (v*u) * u, dt);
//            }
//        }
//    }
//
//    sf::Drawable* render() override {
//        // top_left, m_width, m_height
//        auto* rect = new sf::RectangleShape({m_width, m_height});
//        rect->setOrigin(m_width * 0.5f, m_height * 0.5f);
//        rect->setPosition(top_left + sf::Vector2f{m_width * 0.5f, m_height * 0.5f});
//        rect->setFillColor(color);
//
//        return rect;
//    }
//};