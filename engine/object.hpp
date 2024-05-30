//
// Created by rhseung on 2024-03-27.
//

#pragma once
#define DEBUG

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

#include "../utils/math.hpp"

struct overlap_t {
    sf::Vector2f axis;
    float penetration;
};

class OBB {
 public:
    sf::Vector2f center;
    float width, height;
    float angle;

    OBB(const sf::Vector2f center, const float width, const float height, const float angle=0.f): center{center}, width{width}, height{height}, angle{angle} {}

    [[nodiscard]] sf::Vector2f height_vector() const {  // 박스 중심에서 윗 변 중심으로의 벡터
        return sf::Vector2f{std::cos(angle), std::sin(angle)} * (height / 2);
    }

    [[nodiscard]] sf::Vector2f width_vector() const {   // 박스 중심에서 왼쪽 변 중심으로의 벡터
        return sf::Vector2f{-std::sin(angle), std::cos(angle)} * (width / 2);
    }

    std::vector<sf::Vector2f> vertices() {  // 반시계 방향
        const sf::Vector2f h = height_vector();
        const sf::Vector2f w = width_vector();
        return {center + h + w, center + h - w, center - h - w, center - h + w};
    }

    sf::Vector2f vertex(int i) {
        const sf::Vector2f h = height_vector();
        const sf::Vector2f w = width_vector();
        switch (i) {
            case 0: return center + h + w;
            case 1: return center + h - w;
            case 2: return center - h - w;
            case 3: return center - h + w;
            default: throw std::runtime_error("Invalid vertex index");
        }
    }

    static overlap_t overlap(const OBB& A, const OBB& B) {
        auto overlaps = std::vector<overlap_t>();

        auto A_h = A.height_vector();
        auto A_w = A.width_vector();
        auto B_h = B.height_vector();
        auto B_w = B.width_vector();
        auto d = B.center - A.center;

        for (auto &u : {A_h, A_w, B_h, B_w}) {
            auto a = std::abs(d * u);
            auto b = std::abs(A_h * u) + std::abs(A_w * u) + std::abs(B_h * u) + std::abs(B_w * u);

            if (a > b) {    // 분리축이 존재하면
                return {sf::Vector2f{0.f, 0.f}, 0.f};
            } else
                overlaps.push_back({u, b - a});
        }

        // 가장 penetration이 작은 축을 반환
        float min_overlap = std::numeric_limits<float>::max();
        overlap_t min_axis;
        for (auto &overlap : overlaps) {
            if (overlap.penetration < min_overlap) {
                min_overlap = overlap.penetration;
                min_axis = overlap;
            }
        }

        return min_axis;
    }
};

class Object {
 public:
    sf::Color color = sf::Color::White;
    OBB box;

    // verlet integration
    sf::Vector2f pos;
    sf::Vector2f pos_0;
    sf::Vector2f acc;

    float mass = 1.f;
    float angle = 0.f;

    Object(const sf::Vector2f position, OBB box, float mass=1.f)
        : box{box}, pos{position}, pos_0{position}, acc{0.f, 0.f}, mass{mass}, angle{box.angle} {}

    virtual void update(const float dt) {
        const sf::Vector2f dx = pos - pos_0;
        pos_0 = pos;
        pos = pos + dx + 0.5f * acc * dt * dt;
        acc = {};

        box.center = pos;
        box.angle = angle;
    }

    void accelerate(const sf::Vector2f a) {
        acc += a;
    }

    void set_velocity(const sf::Vector2f v, const float dt) {
        pos_0 = pos - v * dt;
    }

    void add_velocity(const sf::Vector2f v, const float dt) {
        pos_0 -= v * dt;
    }

    [[nodiscard]] sf::Vector2f vel(const float dt) const {
        return (pos - pos_0) / dt;
    }

    virtual std::vector<sf::Drawable*> render() = 0;
};

class CircleObject : public Object {
 public:
    float radius;

    CircleObject(const sf::Vector2f position, const float radius, float mass=1.f, float angle=0.f)
        : Object{position, OBB{position, 2 * radius, 2 * radius, angle}, mass}, radius{radius} {}

    void update(const float dt) {
        const sf::Vector2f dx = pos - pos_0;
        pos_0 = pos;
        pos = pos + dx + 0.5f * acc * dt * dt;
        acc = {};

        // TODO: 회전은 바닥에 닿아야만.
        angle += dx.x / radius;

        box.center = pos;
        box.angle = angle;
    }

    std::vector<sf::Drawable*> render() override {
        auto* circle = new sf::CircleShape(radius);
        circle->setPointCount(32);
        circle->setOrigin(radius, radius);
        circle->setPosition(pos);
        circle->setFillColor(color);

        // radius line(rectangle shape) from pos to (pos + radius * cos(angle), pos + radius * sin(angle))
        auto* line = new sf::RectangleShape({radius, 1.f});
        line->setOrigin(0.f, 0.5f);
        line->setPosition(pos);
        line->setFillColor(sf::Color::White);
        line->setRotation(angle * 180.f / Math::PI);

#ifndef DEBUG
        return {circle, line};
#else
        auto vertices = box.vertices();
        auto obb = new sf::ConvexShape(4);
        for (int i = 0; i < 4; ++i)
            obb->setPoint(i, vertices[i]);
        obb->setFillColor(sf::Color::Transparent);
        obb->setOutlineColor(sf::Color::White);
        obb->setOutlineThickness(1.f);

        return {circle, line, obb};
#endif
    }
};

class RectangleObject : public Object {
 public:
    float width;
    float height;

    RectangleObject(const sf::Vector2f position, const float width, const float height, float mass=1.f, float angle=0.f)
        : Object{position, OBB{position, width, height, angle}, mass}, width{width}, height{height} {}

    std::vector<sf::Drawable*> render() override {
        auto* rectangle = new sf::RectangleShape({width, height});
        rectangle->setOrigin(width * 0.5f, height * 0.5f);
        rectangle->setPosition(pos);
        rectangle->setFillColor(color);

#ifndef DEBUG
        return {rectangle};
#else
        auto vertices = box.vertices();
        auto obb = new sf::ConvexShape(4);
        for (int i = 0; i < 4; ++i)
            obb->setPoint(i, vertices[i]);
        obb->setFillColor(sf::Color::Transparent);
        obb->setOutlineColor(sf::Color::White);
        obb->setOutlineThickness(1.f);

        return {rectangle, obb};
#endif
    }
};