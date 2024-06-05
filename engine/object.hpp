//
// Created by rhseung on 2024-03-27.
//

#pragma once
//#define DEBUG

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

#include "../utils/math.hpp"
#include "../utils/matrix.hpp"
#include "common/Material.hpp"

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

//class Body {
// public:
//    sf::Color color = sf::Color::White;
//    Material material = Materials::wood;
//    OBB box;
//
//    sf::Vector2f position;
//    sf::Vector2f pos_0;
//    sf::Vector2f pos_0_0;
//    sf::Vector2f velocity;
//    sf::Vector2f acceleration;
//
//    float mass = 1.f;
//    float angle = 0.f;
//
//    Body(const sf::Vector2f position, OBB box, float mass=1.f, Material material=Materials::wood)
//        : box{box}, position{position}, pos_0{position}, pos_0_0{position}, acceleration{0.f, 0.f}, mass{mass}, angle{box.angle},
//          material{material}, color{material.color} {}
//
//    virtual void update(const float dt) {
//        const sf::Vector2f dx = position - pos_0;
//        pos_0 = position;
//        pos_0_0 = pos_0;
//        position = position + dx + 0.5f * acceleration * dt * dt;
//        acceleration = {};
//
//        box.center = position;
//        box.angle = angle;
//    }
//
//    void accelerate(const sf::Vector2f a) {
//        acceleration += a;
//    }
//
//    void set_velocity(const sf::Vector2f v, const float dt) {
//        pos_0 = position - v * dt;
//    }
//
//    void add_velocity(const sf::Vector2f v, const float dt) {
//        pos_0 -= v * dt;
//    }
//
//    [[nodiscard]] sf::Vector2f vel(const float dt) const {
//        return (position - pos_0) / dt;
//    }
//
//    virtual std::vector<sf::Drawable*> render() = 0;
//};
//
//class CircleObject : public Body {
// public:
//    float radius;
//
//    CircleObject(const sf::Vector2f position, const float radius, Material material=Materials::wood, float mass=1.f, float angle=0.f)
//        : Body{position, OBB{position, 2 * radius, 2 * radius, angle}, mass, material}, radius{radius} {}
//
//    void update(const float dt) {
//        const sf::Vector2f dx = position - pos_0;
//        pos_0 = position;
//        pos_0_0 = pos_0;
//        position = position + dx + 0.5f * acceleration * dt * dt;
//        acceleration = {};
//
//        // TODO: 회전은 바닥에 닿아야만.
////        angle += std::abs(position - pos_0) / radius;
//        // 시계방향 반시계 방향 고려
//
////        auto [a, b] = position;
////        auto [c, d] = pos_0;
////        auto [e, f] = pos_0_0;
////
////        std::vector<std::vector<float>> matrix = {
////            {a, b, 1},
////            {c, d, 1},
////            {e, f, 1}
////        };
////        auto solved = solve_3linear(matrix, -a*a-b*b, -c*c-d*d, -e*e-f*f);
////        auto D = solved[0], E = solved[1], F = solved[2];
////        sf::Vector2f center = {-D/2, -E/2};
//        // TODO: 회전 구현은 나중에
//        sf::Vector2f center = {500.f, 400.f};
//        auto ccw = Math::cross(position - center, pos_0 - center);
//        if (ccw > 0) angle += std::abs(position - pos_0) / radius;   // 반시계로 움직이면 시계방향으로 구름
//        else angle -= std::abs(position - pos_0) / radius;
//
//        box.center = position;
//        box.angle = angle;
//    }
//
//    std::vector<sf::Drawable*> render() override {
//        auto* circle = new sf::CircleShape(radius);
//        circle->setPointCount(32);
//        circle->setOrigin(radius, radius);
//        circle->setPosition(position);
//        circle->setFillColor(color);
//        circle->setOutlineThickness(2.f);
//
//        // radius line(rectangle shape) from position to (position + radius * cos(angle), position + radius * sin(angle))
//        auto* line = new sf::RectangleShape({radius, 1.f});
//        line->setOrigin(0.f, 0.5f);
//        line->setPosition(position);
//        line->setFillColor(sf::Color::White);
//        line->setRotation(angle * 180.f / Math::PI);
//        line->setOutlineThickness(1.f);
//
//#ifndef DEBUG
//        return {circle, line};
//#else
//        auto vertices = box.vertices();
//        auto obb = new sf::ConvexShape(4);
//        for (int i = 0; i < 4; ++i)
//            obb->setPoint(i, vertices[i]);
//        obb->setFillColor(sf::Color::Transparent);
//        obb->setOutlineColor(sf::Color::White);
//        obb->setOutlineThickness(1.f);
//
//        return {circle, line, obb};
//#endif
//    }
//};

//class RectangleObject : public Body {
// public:
//    float width;
//    float height;
//
//    RectangleObject(const sf::Vector2f position, const float width, const float height, Material material=Materials::wood, float mass=1.f, float angle=0.f)
//        : Body{position, OBB{position, width, height, angle}, mass, material}, width{width}, height{height} {}
//
//    std::vector<sf::Drawable*> render() override {
//        auto* rectangle = new sf::RectangleShape({width, height});
//        rectangle->setOrigin(width * 0.5f, height * 0.5f);
//        rectangle->setPosition(position);
//        rectangle->setFillColor(color);
//
//#ifndef DEBUG
//        return {rectangle};
//#else
//        auto vertices = box.vertices();
//        auto obb = new sf::ConvexShape(4);
//        for (int i = 0; i < 4; ++i)
//            obb->setPoint(i, vertices[i]);
//        obb->setFillColor(sf::Color::Transparent);
//        obb->setOutlineColor(sf::Color::White);
//        obb->setOutlineThickness(1.f);
//
//        return {rectangle, obb};
//#endif
//    }
//};