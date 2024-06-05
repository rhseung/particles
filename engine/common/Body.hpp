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
constexpr float PI = std::numbers::pi_v<float>;

class Body {
 public:
    Vec2 force;
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float angle_acceleration = 0.f;
    float angle_velocity = 0.f;
    float angle = 0.f;
    const Material material;
    const ShapeType shape;
    sf::Color color = material.color;
    bool is_static = false;
    float max_speed = std::numeric_limits<float>::infinity();

    virtual void update(float dt) {
        acceleration += force / mass();
        Vec2 new_velocity = velocity + acceleration * dt;
        if (std::abs(new_velocity.x) <= max_speed && std::abs(new_velocity.y) <= max_speed)
            velocity = new_velocity;
        position += velocity * dt;

        angle_velocity += angle_acceleration * dt;
        angle += angle_velocity * dt;

        acceleration = {};
        force = {};
    }

    virtual List<sf::Drawable *> render() = 0;

    [[nodiscard]] virtual float mass() const {
        if (is_static)
            return std::numeric_limits<float>::infinity();
        else
            return 0;
    }

    [[nodiscard]] float inverseMass() const {
        if (is_static)
            return 0;
        else
            return 1.f / mass();
    }

    [[nodiscard]] virtual float inertia() const = 0;

    virtual void move(Vec2 movement) {
        position += movement;
    }

    void faster(Vec2 velocity) {
        Vec2 new_velocity = this->velocity + velocity;
        if (std::abs(new_velocity.x) <= max_speed && std::abs(new_velocity.y) <= max_speed)
            this->velocity = new_velocity;
    }

    void accelerate(Vec2 acceleration) {
        this->acceleration += acceleration;
    }

    virtual void rotate(float angle) {
        this->angle += angle;
    }

    void applyForce(Vec2 force) {
        this->force += force;
    }

    [[nodiscard]] float speed() const {
        return std::abs(velocity);
    }

    void setMaxSpeed(float speed) {
        max_speed = speed;
    }

    Body(Vec2 position, Material material, ShapeType shape, sf::Color color, bool is_static = false)
        : position{position}, material{material}, shape{shape}, color{color}, is_static{is_static} {}
};

class CircleBody : public Body {
 public:
    float radius;

    CircleBody(Vec2 position, float radius, Material material, sf::Color color, bool is_static = false)
        : Body{position, material, ShapeType::CIRCLE, color, is_static}, radius{radius} {}

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), material.density * std::numbers::pi_v<float> * (radius * radius));
    }

    [[nodiscard]] float inertia() const override {
        return 0.5f * mass() * radius * radius;
    }

    List<sf::Drawable *> render() override {
        auto *circle = new sf::CircleShape(radius - 1);
        circle->setPointCount(32);
        circle->setOrigin(radius, radius);
        circle->setPosition(position);
        circle->setFillColor(color);
        circle->setOutlineThickness(2.f);
        if (is_static)
            circle->setOutlineColor(sf::Color{70, 70, 70});

        auto *line = new sf::RectangleShape({radius - 1, 1.f});
        line->setOrigin(0.f, 0.5f);
        line->setPosition(position);
        line->setFillColor(sf::Color::White);
        line->setRotation(angle * 180.f / PI);
        line->setOutlineThickness(0.5f);
        if (is_static) {
            line->setOutlineColor(sf::Color{70, 70, 70});
            line->setFillColor(sf::Color{70, 70, 70});
        }

        return {circle, line};
    }
};

class PolygonBody : public Body {
 public:
    List<Vec2> vertices;

    PolygonBody(Vec2 position, List<Vec2> vertices, Material material, sf::Color color, bool is_static = false)
        : Body{position, material, ShapeType::POLYGON, color, is_static}, vertices{std::move(vertices)} {
        angle = PI / 2;
    }

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), material.density * 1);
        // TODO: Implement mass calculation for polygons
    }

    [[nodiscard]] float inertia() const override {
        return 1;
        // TODO: Implement inertia calculation for polygons
    }

    void update(float dt) override {
        Body::update(dt);

        // vertices update
        for (auto &vertex : vertices) {
            vertex += velocity * dt;
        }
    }

    List<sf::Drawable *> render() override {
        auto *poly = new sf::ConvexShape(vertices.size());
        poly->setFillColor(color);
        poly->setOutlineThickness(2.f);

        if (is_static)
            poly->setOutlineColor(sf::Color{70, 70, 70});

        for (int i = 0; i < vertices.size(); ++i) {
            auto n = Math::normalize(vertices[i] - position);
            poly->setPoint(i, vertices[i] - n * 2.f);
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

    RectangleBody(Vec2 position, float width, float height, Material material, sf::Color color, bool is_static = false)
        : PolygonBody{position, {}, material, color, is_static}, width{width}, height{height} {
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
        return std::max(Body::mass(), material.density * width * height);
    }

    [[nodiscard]] float inertia() const override {
        return mass() * (width * width + height * height) / 12.f;
    }
};

class RegularPolygonBody : public PolygonBody {
 public:
    float radius;
    int sides;

    RegularPolygonBody(Vec2 position, float radius, int sides, Material material, sf::Color color, bool is_static = false)
        : PolygonBody{position, {}, material, color, is_static}, radius{radius}, sides{sides} {
        if (sides < 3)
            throw std::invalid_argument("RegularPolygonBody: sides must be >= 3");

        for (int i = 0; i < sides; ++i) {
            float angle = 2.f * PI * i / sides;
            vertices.emplace_back(position + radius * Vec2{std::cos(angle), std::sin(angle)});
        }
    }

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), material.density * radius * radius * sides * std::sin(PI / sides));
    }

    [[nodiscard]] float inertia() const override {
        return mass() * radius * radius / 2;
    }
};