#pragma once
#include <cmath>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../utils/math.hpp"
#include "object.hpp"
#include "constraints.hpp"

struct VerletObject {
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    float radius = 10.0f;
    sf::Color color = sf::Color::White;

    VerletObject() = default;
    VerletObject(sf::Vector2f position_, float radius_)
        : position{position_}, position_last{position_}, acceleration{0.0f, 0.0f}, radius{radius_} {}

    void update(float dt) {
        // Compute how much we moved
        const sf::Vector2f displacement = position - position_last;
        // Update position
        position_last = position;
        position = position + displacement + acceleration * (dt * dt);
        // Reset acceleration
        acceleration = {};
    }

    void accelerate(sf::Vector2f a) {
        acceleration += a;
    }

    void setVelocity(sf::Vector2f v, float dt) {
        position_last = position - (v * dt);
    }

    void addVelocity(sf::Vector2f v, float dt) {
        position_last -= v * dt;
    }

    [[nodiscard]]
    sf::Vector2f getVelocity(float dt) const {
        return (position - position_last) / dt;
    }
};

class Solver {
 public:
    Solver() = default;

    Object& addObject(Object* obj) {
        m_objects.push_back(obj);
        return *obj;
    }

    void update() {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();
        for (uint32_t i{m_sub_steps}; i--;) {
            applyGravity();
            checkCollisions(step_dt);
            applyConstraint();
            updateObjects(step_dt);
        }
    }

    void setSimulationUpdateRate(uint32_t rate) {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void addConstraint(Constraint* constraint) {
        m_constraints.push_back(constraint);
    }

    void setSubStepsCount(uint32_t sub_steps) {
        m_sub_steps = sub_steps;
    }

    void setObjectVelocity(Object &object, sf::Vector2f v) const {
        object.set_velocity(v, getStepDt());
    }

    [[nodiscard]]
    const std::vector<Object*> &getObjects() const {
        return m_objects;
    }

    [[nodiscard]]
    const std::vector<Constraint*> &getConstraints() const {
        return m_constraints;
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const {
        return m_objects.size();
    }

    [[nodiscard]]
    float getTime() const {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const {
        return m_frame_dt / static_cast<float>(m_sub_steps);
    }

 private:
    std::vector<Object*> m_objects;
    std::vector<Constraint*> m_constraints;

    uint32_t m_sub_steps = 1;
    sf::Vector2f m_gravity = {0.0f, 1500.0f};
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    void applyGravity() {
        for (auto &obj : m_objects) {
            obj->accelerate(m_gravity);
        }
    }

    void checkCollisions(float dt) {
        const float response_coef = 0.75f;
        const uint64_t objects_count = m_objects.size();

        for (int i = 0; i < objects_count; ++i) {
            for (int j = i + 1; j < objects_count; ++j) {
                auto box1 = m_objects[i]->box;
                auto box2 = m_objects[j]->box;
                auto overlap = OBB::overlap(box1, box2);
                if (overlap.penetration > 0) {
                    const sf::Vector2f n = Math::normalize(overlap.axis);
                    const float mass_ratio_1 = m_objects[i]->mass / (m_objects[i]->mass + m_objects[j]->mass);
                    const float mass_ratio_2 = m_objects[j]->mass / (m_objects[i]->mass + m_objects[j]->mass);
                    const float delta = 0.5f * response_coef * overlap.penetration;

                    m_objects[i]->pos -= n * (mass_ratio_2 * delta);
                    m_objects[j]->pos += n * (mass_ratio_1 * delta);
                }
            }
        }

        // TODO: 원은 반지름으로 계산하는게 더 나음
        //  - 모양에 따른 collsion detection type를 나누자.

//        for (uint64_t i{0}; i < objects_count; ++i) {
//            auto &object_1 = dynamic_cast<CircleObject &>(*m_objects[i]);
//            // Iterate on object involved in new collision pairs
//            for (uint64_t k{i + 1}; k < objects_count; ++k) {
//                auto &object_2 = dynamic_cast<CircleObject &>(*m_objects[k]);
//                const sf::Vector2f v = object_1.pos - object_2.pos;
//                const float dist2 = v.x * v.x + v.y * v.y;
//                const float min_dist = object_1.radius + object_2.radius;
//                // Check overlapping
//                if (dist2 < min_dist * min_dist) {
//                    const float dist = std::sqrt(dist2);
//                    const sf::Vector2f n = v / dist;
//                    const float mass_ratio_1 = object_1.mass / (object_1.mass + object_2.mass);
//                    const float mass_ratio_2 = object_2.mass / (object_1.mass + object_2.mass);
//                    const float delta = 0.5f * response_coef * (dist - min_dist);
//                    // Update positions
//                    object_1.pos -= n * (mass_ratio_2 * delta);
//                    object_2.pos += n * (mass_ratio_1 * delta);
//                }
//            }
//        }
    }

    void applyConstraint() {
        for (Object *obj : m_objects) {
            for (Constraint *constraint : m_constraints) {
                constraint->apply(obj, getStepDt());
            }
        }
    }

    void updateObjects(float dt) {
        for (auto &obj : m_objects) {
            obj->update(dt);
        }
    }
};
