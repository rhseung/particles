#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <numbers>

#include "Material.hpp"
#include "Shape.hpp"
#include "../../utils/math.hpp"
#include "../../utils/colors.hpp"

template<typename T>
using List = std::vector<T>;
using Vec2 = sf::Vector2f;

class Body {
 protected:
    Vec2 m_force;
    Vec2 m_position;
    Vec2 m_velocity;
    Vec2 m_acceleration;
    float m_angular_acceleration = 0.f;
    float m_angular_velocity = 0.f;
    float m_angle = 0.f;
    const Material m_material;
    const ShapeType m_shape;
    sf::Color m_color = m_material.color;
    sf::Color m_outline_color = whiteOrBlack(m_color);
    bool m_is_static = false;
    float m_max_speed = std::numeric_limits<float>::infinity();
    float m_max_angular_speed = std::numeric_limits<float>::infinity();

    virtual void linearTransform(Vec2 dx) = 0;
    virtual void angularTransform(float d_angle) = 0;

 public:
    Body(Vec2 position, Material material, ShapeType shape)
        : m_position{position}, m_material{material}, m_shape{shape} {}

    virtual void update(float dt) {
        accelerate(m_force / mass());
        addVelocity(m_acceleration * dt);
        move(m_velocity * dt);

//        addAngularAcceleration(torque);
        addAngularVelocity(m_angular_acceleration * dt);
        rotate(m_angular_velocity * dt);

        setAcceleration({});
        setForce({});
    }
    virtual List<sf::Drawable *> render() = 0;

    [[nodiscard]] virtual float mass() const {
        if (m_is_static)
            return std::numeric_limits<float>::infinity();
        else
            return 0;
    }
    [[nodiscard]] virtual float inertia() const = 0;

    Body& addForce(Vec2 force) {
        m_force += force;
        return *this;
    }
    Body& setForce(Vec2 force) {
        m_force = force;
        return *this;
    }
    [[nodiscard]] Vec2 force() const {
        return m_force;
    }

    Body& addPosition(Vec2 movement) {
        m_position += movement;
        linearTransform(movement);
        return *this;
    }
    Body& setPosition(Vec2 position) {
        return addPosition(position - m_position);
    }
    [[nodiscard]] Vec2 position() const {
        return m_position;
    }

    Body& addVelocity(Vec2 velocity) {
        Vec2 new_velocity = m_velocity + velocity;
        if (std::abs(new_velocity) <= m_max_speed)
            m_velocity = new_velocity;

        return *this;
    }
    Body& setVelocity(Vec2 velocity) {
        if (std::abs(velocity) <= m_max_speed)
            m_velocity = velocity;
        else
            m_velocity = Math::normalize(velocity) * m_max_speed;

        return *this;
    }
    [[nodiscard]] Vec2 velocity() const {
        return m_velocity;
    }

    Body& addAcceleration(Vec2 acceleration) {
        m_acceleration += acceleration;
        return *this;
    }
    Body& setAcceleration(Vec2 acceleration) {
        m_acceleration = acceleration;
        return *this;
    }
    [[nodiscard]] Vec2 acceleration() const {
        return m_acceleration;
    }

    Body& addAngle(float angle) {
        m_angle += angle;
        angularTransform(angle);
        return *this;
    }
    Body& setAngle(float angle) {
        return addAngle(angle - m_angle);
    }
    [[nodiscard]] float angle() const {
        return m_angle;
    }

    Body& addAngularVelocity(float angular_velocity) {
        float new_angle_velocity = m_angular_velocity + angular_velocity;
        if (std::abs(new_angle_velocity) <= m_max_angular_speed)
            m_angular_velocity = new_angle_velocity;

        return *this;
    }
    Body& setAngularVelocity(float angular_velocity) {
        m_angular_velocity = std::min(angular_velocity, m_max_angular_speed);
        return *this;
    }
    [[nodiscard]] float angularVelocity() const {
        return m_angular_velocity;
    }

    Body& addAngularAcceleration(float angular_acceleration) {
        m_angular_acceleration += angular_acceleration;
        return *this;
    }
    Body& setAngularAcceleration(float angular_acceleration) {
        m_angular_acceleration = angular_acceleration;
        return *this;
    }
    [[nodiscard]] float angularAcceleration() const {
        return m_angular_acceleration;
    }

    [[nodiscard]] Material material() const {
        return m_material;
    }

    [[nodiscard]] ShapeType shape() const {
        return m_shape;
    }

    Body& setColor(sf::Color color) {
        m_color = color;
        m_outline_color = whiteOrBlack(color);
        return *this;
    }
    [[nodiscard]] sf::Color color() const {
        return m_color;
    }

    Body& setOutlineColor(sf::Color color) {
        m_outline_color = color;
        return *this;
    }
    [[nodiscard]] sf::Color outlineColor() const {
        return m_outline_color;
    }

    Body& setStatic(bool is_static) {
        m_is_static = is_static;
        return *this;
    }
    [[nodiscard]] bool isStatic() const {
        return m_is_static;
    }

    Body& setMaxSpeed(float speed) {
        m_max_speed = speed;
        return *this;
    }
    [[nodiscard]] float maxSpeed() const {
        return m_max_speed;
    }

    Body& setMaxAngularSpeed(float angular_speed) {
        m_max_angular_speed = angular_speed;
        return *this;
    }
    [[nodiscard]] float maxAngularSpeed() const {
        return m_max_angular_speed;
    }

    // friendly functions
    Body& move(Vec2 movement) {
        return addPosition(movement);
    }
    Body& moveTo(Vec2 position) {
        return setPosition(position);
    }
    Body& accelerate(Vec2 acceleration) {
        return addAcceleration(acceleration);
    }
    Body& rotate(float angle) {
        return addAngle(angle);
    }
    Body& rotateTo(float angle) {
        return setAngle(angle);
    }
    virtual bool contains(Vec2 point) = 0;
    [[nodiscard]] virtual bool isConvex() const = 0;
    [[nodiscard]] bool isConcave() const {
        return !isConvex();
    }

    // etc
    [[nodiscard]] float speed() const {
        return std::abs(m_velocity);
    }
    [[nodiscard]] float inverseMass() const {
        if (m_is_static)
            return 0;
        else
            return 1.f / mass();
    }
};

class CircleBody : public Body {
 protected:
    const float m_radius;

    void linearTransform(Vec2 dx) override {};
    void angularTransform(float d_angle) override {};
 public:
    CircleBody(Vec2 position, float radius, Material material)
        : Body{position, material, ShapeType::CIRCLE}, m_radius{radius} {}

    [[nodiscard]] bool isConvex() const override {
        return true;
    }
    bool contains(Vec2 point) override {
        return std::abs(position() - point) <= radius();
    }

    [[nodiscard]] float radius() const {
        return m_radius;
    }

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), m_material.density * std::numbers::pi_v<float> * (m_radius * m_radius));
    }
    [[nodiscard]] float inertia() const override {
        return 0.5f * mass() * m_radius * m_radius;
    }

    List<sf::Drawable *> render() override {
        auto *circle = new sf::CircleShape(radius() - 1);
        circle->setPointCount(32);
        circle->setOrigin(radius(), radius());
        circle->setPosition(position());
        circle->setFillColor(color());
        circle->setOutlineThickness(2.f);
        circle->setOutlineColor(outlineColor());

        auto *line = new sf::RectangleShape({radius() - 1, 1.f});
        line->setOrigin(0.f, 0.5f);
        line->setPosition(position());
        line->setFillColor(outlineColor());
        line->setRotation(angle() * 180.f / Math::PI);
        line->setOutlineThickness(0.5f);
        line->setOutlineColor(outlineColor());

        return {circle, line};
    }
};

class PolygonBody : public Body {
 protected:
    List<Vec2> m_vertices;

    void linearTransform(Vec2 dx) override {
        for (auto &vertex : m_vertices) {
            vertex += dx;
        }
    };
    void angularTransform(float d_angle) override {
        for (auto &vertex : m_vertices) {
            vertex = Math::rotate(vertex, d_angle, m_position);
        }
    };
 public:
    PolygonBody(Vec2 position, List<Vec2> vertices, Material material)
        : Body{position, material, ShapeType::POLYGON}, m_vertices{std::move(vertices)} {
        setAngle(Math::PI / 2);
    }

    [[nodiscard]] bool isConvex() const override {
        for (int i = 0; i < m_vertices.size(); ++i) {
            Vec2 p1 = m_vertices[i];
            Vec2 p2 = m_vertices[(i + 1) % m_vertices.size()];
            Vec2 p3 = m_vertices[(i + 2) % m_vertices.size()];

            if (Math::ccw(p1, p2, p3) < 0)
                return false;
        }

        return true;
    }
    bool contains(Vec2 point) override {
        // https://anz1217.tistory.com/107
        if (isConcave()) {
            int count = 0;
            for (int i = 0; i < m_vertices.size(); ++i) {
                Vec2 p1 = m_vertices[i];
                Vec2 p2 = m_vertices[(i + 1) % m_vertices.size()];

                if (p1.y < p2.y)
                    std::swap(p1, p2);

                Vec2 v1 = point - p1;
                Vec2 v2 = p2 - point;

                int ccw = Math::ccw(p1, p2);
                if (ccw == 0) {
                    if (std::min(p1.x, p2.x) <= point.x && point.x <= std::max(p1.x, p2.x)
                        && std::min(p1.y, p2.y) <= point.y && point.y <= std::max(p1.y, p2.y))
                        return true;
                }

                if (std::max(p1.x, p2.x) < point.x) continue;
                if (p1.y <= point.y) continue;
                if (point.y < p2.y) continue;
                if (ccw > 0) count++;
            }

            return count % 2 == 1;
        }
        else {
            Vec2 vec_l = m_vertices[sides() - 1] - m_vertices[0];
            Vec2 vec_r = m_vertices[1] - m_vertices[0];
            Vec2 vec_p = point - m_vertices[0];

            if (Math::ccw(vec_l, vec_p) > 0)
                return false;
            if (Math::ccw(vec_r, vec_p) < 0)
                return false;

            int l = 1, r = sides() - 1;
            while (l + 1 < r) {
                int mid = (l + r) / 2;
                Vec2 vec_m = m_vertices[mid] - m_vertices[0];
                if (Math::ccw(vec_m, vec_p) > 0)
                    l = mid;
                else
                    r = mid;

            }

            Vec2 v1 = point - m_vertices[l];
            Vec2 v2 = m_vertices[l + 1] - point;
            return Math::ccw(v1, v2) < 0;
        }
    }

    [[nodiscard]] List<Vec2> vertices() const {
        return m_vertices;
    }
    [[nodiscard]] Vec2 vertex(int index) const {
        return m_vertices[index];
    }
    [[nodiscard]] uint64_t sides() const {
        return m_vertices.size();
    }

    [[nodiscard]] float mass() const override {
        float area = 0;
        for (int i = 0; i < m_vertices.size(); ++i) {
            Vec2 p1 = m_vertices[i];
            Vec2 p2 = m_vertices[(i + 1) % m_vertices.size()];
            area += Math::cross(p1 - position(), p2 - position()) / 2.f;
        }

        return std::max(Body::mass(), material().density * area);
    }
    [[nodiscard]] float inertia() const override {
        float inertia = 0;
        for (int i = 0; i < m_vertices.size(); ++i) {
            Vec2 p1 = m_vertices[i];
            Vec2 p2 = m_vertices[(i + 1) % m_vertices.size()];
            inertia += Math::cross(p1, p2) * (Math::dot(p1, p1) + Math::dot(p1, p2) + Math::dot(p2, p2));
        }

        // TODO: 정확하지 않음
        return mass() * inertia / 6.f;
    };

    List<sf::Drawable *> render() override {
        auto *poly = new sf::ConvexShape(sides());
        poly->setFillColor(color());
        poly->setOutlineThickness(2.f);
        poly->setOutlineColor(outlineColor());

        for (int i = 0; i < m_vertices.size(); ++i) {
            auto n = Math::normalize(m_vertices[i] - position());
            poly->setPoint(i, m_vertices[i] - n * 2.f);
        }

        return {poly};
    }
};

class RectangleBody : public PolygonBody {
 protected:
    const float m_width, m_height;
 public:
    RectangleBody(Vec2 position, float width, float height, Material material)
        : PolygonBody{position, {}, material}, m_width{width}, m_height{height} {
        const sf::Vector2f h = heightVec();
        const sf::Vector2f w = widthVec();
        m_vertices = {position - h + w, position + h + w, position + h - w, position - h - w};  // 반시계방향
    }

    [[nodiscard]] bool isConvex() const override {
        return true;
    }
    bool contains(Vec2 point) override {
        Vec2 p1 = m_vertices[0];
        Vec2 p2 = m_vertices[1];
        Vec2 p3 = m_vertices[2];
        Vec2 p4 = m_vertices[3];

        if (Math::ccw(p1, p2, point) < 0) return false;
        if (Math::ccw(p2, p3, point) < 0) return false;
        if (Math::ccw(p3, p4, point) < 0) return false;
        if (Math::ccw(p4, p1, point) < 0) return false;

        return true;
    }

    [[nodiscard]] float width() const {
        return m_width;
    }
    [[nodiscard]] float height() const {
        return m_height;
    }

    [[nodiscard]] sf::Vector2f heightVec() const {  // 박스 중심에서 윗 변 중심으로의 벡터
        return sf::Vector2f{std::cos(m_angle), std::sin(m_angle)} * (m_height / 2);
    }
    [[nodiscard]] sf::Vector2f widthVec() const {   // 박스 중심에서 왼쪽 변 중심으로의 벡터
        return sf::Vector2f{-std::sin(m_angle), std::cos(m_angle)} * (m_width / 2);
    }

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), m_material.density * m_width * m_height);
    }
    [[nodiscard]] float inertia() const override {
        return mass() * (m_width * m_width + m_height * m_height) / 12.f;
    }
};

class RegularPolygonBody : public PolygonBody {
 protected:
    float m_radius;
 public:
    RegularPolygonBody(Vec2 position, float radius, uint64_t sides, Material material)
        : PolygonBody{position, {}, material}, m_radius{radius} {
        if (sides < 3)
            throw std::invalid_argument("RegularPolygonBody: sides must be >= 3");

        for (uint64_t i = 0; i < sides; ++i) {
            float angle = 2.f * Math::PI * (float)i / sides;
            m_vertices.emplace_back(position + radius * Vec2{std::cos(angle), std::sin(angle)});
        }
    }

    [[nodiscard]] bool isConvex() const override {
        return true;
    }

    [[nodiscard]] float radius() const {
        return m_radius;
    }
    [[nodiscard]] float length() const {
        return 2.f * m_radius * std::sin(Math::PI / sides());
    }

    [[nodiscard]] float mass() const override {
        return std::max(Body::mass(), m_material.density * m_radius * m_radius * sides() * std::sin(Math::PI / sides()));
    }
    [[nodiscard]] float inertia() const override {
        // ref: https://m.blog.naver.com/PostView.naver?blogId=gks36247&logNo=221548621791&fromRecommendationType=category&targetRecommendationDetailCode=1000
        float tan = std::tan(Math::PI / sides());
        float beta = 1.f/8 * (1.f/3 + 1.f/(tan*tan));
        return beta * mass() * length() * length();
    }
};