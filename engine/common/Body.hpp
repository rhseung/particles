#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <numbers>

#include "Material.hpp"
#include "Shape.hpp"
#include "../../utils/math.hpp"

template<typename T>
using List = std::vector<T>;
using Vec2 = sf::Vector2f;

class Body {
 public:
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float angle_acceleration = 0.f;
    float angle_velocity = 0.f;
    float angle = 0.f;

    Vec2 force;

    const Material material;
    const ShapeType shape;

    sf::Color color = material.color;

    [[nodiscard]] virtual float mass() const = 0;
    [[nodiscard]] virtual float inertia() const = 0;

    virtual void update(float dt) {
        acceleration += force / mass();
        velocity += acceleration * dt;
        position += velocity * dt;
    }
    virtual List<sf::Drawable *> render() = 0;

    virtual void move(Vec2 movement) {
        position += movement;
    }

    void faster(Vec2 velocity) {
        this->velocity += velocity;
    }

    void accelerate(Vec2 acceleration) {
        this->acceleration += acceleration;
    }

    virtual void rotate(float angle) {
        this->angle += angle;
    }

    void apply_force(Vec2 force) {
        this->force += force;
    }

    Body(Vec2 position, Material material, ShapeType shape, sf::Color color)
        : position{position}, material{material}, shape{shape}, color{color} {}
};

class CircleBody : public Body {
 public:
    float radius;

    CircleBody(Vec2 position, float radius, Material material, sf::Color color)
        : Body{position, material, ShapeType::CIRCLE, color}, radius{radius} {}

    [[nodiscard]] float mass() const override {
        return material.density * std::numbers::pi_v<float> * (radius * radius);
    }

    [[nodiscard]] float inertia() const override {
        return 0.5f * mass() * radius * radius;
    }

    void update(float dt) override {
        acceleration += force / mass();
        velocity += acceleration * dt;
        position += velocity * dt;

        float torque = radius * std::abs(force);
        angle_acceleration += torque / inertia();
        angle_velocity += angle_acceleration * dt;
        angle += angle_velocity * dt;
    }

    List<sf::Drawable *> render() override {
        auto *circle = new sf::CircleShape(radius);
        circle->setPointCount(32);
        circle->setOrigin(radius, radius);
        circle->setPosition(position);
        circle->setFillColor(color);
        circle->setOutlineThickness(2.f);

        auto *line = new sf::RectangleShape({radius, 1.f});
        line->setOrigin(0.f, 0.5f);
        line->setPosition(position);
        line->setFillColor(sf::Color::White);
        line->setRotation(angle * 180.f / std::numbers::pi);
        line->setOutlineThickness(1.f);

        return {circle, line};
    }
};

class PolygonBody : public Body {
 public:
    List<Vec2> vertices;

    PolygonBody(Vec2 position, List<Vec2> vertices, Material material, sf::Color color)
        : Body{position, material, ShapeType::POLYGON, color}, vertices{std::move(vertices)} {}

    [[nodiscard]] float mass() const override {
        return material.density * 1;
        // TODO: Implement mass calculation for polygons
    }

    [[nodiscard]] float inertia() const override {
        return 1;
        // TODO: Implement inertia calculation for polygons
    }

    void update(float dt) override {
        acceleration += force / mass();
        velocity += acceleration * dt;
        position += velocity * dt;

        // vertices update
        for (auto &vertex : vertices) {
            vertex += velocity * dt;
        }
    }

    List<sf::Drawable *> render() override {
        auto *poly = new sf::ConvexShape(vertices.size());
        poly->setFillColor(color);
        poly->setOutlineThickness(2.f);

        for (int i = 0; i < vertices.size(); ++i) {
            poly->setPoint(i, vertices[i]);
        }

        return {poly};
    }

    void move(Vec2 movement) override {
        position += movement;
        for (auto &vertex : vertices) {
            vertex += movement;
        }
    }

    void rotate(float angle) override {
        this->angle += angle;
        for (auto &vertex : vertices) {
            vertex = Math::rotate(vertex, angle, position);
        }
    }
};

class RectangleBody : public PolygonBody {
 public:
    float width, height;

    RectangleBody(Vec2 position, float width, float height, Material material, sf::Color color)
        : PolygonBody{position, {}, material, color}, width{width}, height{height} {
        const sf::Vector2f h = height_vector();
        const sf::Vector2f w = width_vector();
        vertices = {position + h + w, position + h - w, position - h - w, position - h + w};
    }

    [[nodiscard]] sf::Vector2f height_vector() const {  // 박스 중심에서 윗 변 중심으로의 벡터
        return sf::Vector2f{std::cos(angle), std::sin(angle)} * (height / 2);
    }

    [[nodiscard]] sf::Vector2f width_vector() const {   // 박스 중심에서 왼쪽 변 중심으로의 벡터
        return sf::Vector2f{-std::sin(angle), std::cos(angle)} * (width / 2);
    }

    [[nodiscard]] float mass() const override {
        return material.density * width * height;
    }

    [[nodiscard]] float inertia() const override {
        return mass() * (width * width + height * height) / 12.f;
    }

//    List<sf::Drawable *> render() override {
//        auto *rect = new sf::RectangleShape({width, height});
//        rect->setOrigin(width / 2, height / 2);
//        rect->setPosition(position);
//        rect->setFillColor(color);
//        rect->setOutlineThickness(2.f);
//
//        return {rect};
//    }
};