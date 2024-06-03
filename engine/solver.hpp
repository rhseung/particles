#pragma once
#include <cmath>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../utils/math.hpp"
//#include "object.hpp"
#include "constraints.hpp"
#include "common/Body.hpp"
#include "Collisions.hpp"

class Solver {
 private:
    List<Body*> body_list;
    List<Constraint*> constraint_list;

    unsigned int sub_steps = 1;
    sf::Vector2f gravity = {0.0f, 1500.0f};
    float m_time = 0.0f;
    float m_frame_dt = 0.0f;

    void apply_gravity() {
        for (auto &obj : body_list) {
            obj->accelerate(gravity);
        }
    }

    void check_collisions(float dt) {
        for (unsigned int i = 0; i < body_list.size(); i++) {
            for (unsigned int j = i + 1; j < body_list.size(); j++) {
                if (body_list[i]->shape == ShapeType::CIRCLE) {
                    auto obj1 = dynamic_cast<CircleBody*>(body_list[i]);

                    if (body_list[j]->shape == ShapeType::CIRCLE) {
                        auto obj2 = dynamic_cast<CircleBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersect_circles(obj1->position, obj1->radius, obj2->position, obj2->radius, manifold)) {
                            float mass_sum = obj1->mass() + obj2->mass();
                            float obj1_ratio = obj1->mass() / mass_sum;
                            float obj2_ratio = obj2->mass() / mass_sum;

                            obj1->move(-manifold.normal * 0.5f * manifold.depth * obj1_ratio);
                            obj2->move(manifold.normal * 0.5f * manifold.depth * obj2_ratio);
                        }
                    }
                }
                else if (body_list[i]->shape == ShapeType::POLYGON) {
                    auto obj1 = dynamic_cast<PolygonBody*>(body_list[i]);

                    if (body_list[j]->shape == ShapeType::POLYGON) {
                        auto obj2 = dynamic_cast<PolygonBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersect_polys(obj1->position, obj1->vertices, obj2->position, obj2->vertices, manifold)) {
                            float mass_sum = obj1->mass() + obj2->mass();
                            float obj1_ratio = obj1->mass() / mass_sum;
                            float obj2_ratio = obj2->mass() / mass_sum;

                            obj1->move(-manifold.normal * 0.5f * manifold.depth * obj1_ratio);
                            obj2->move(manifold.normal * 0.5f * manifold.depth * obj2_ratio);
                        }
                    }
                }
            }
        }
    }

    void apply_constraints() {
        for (Body *obj : body_list) {
            for (Constraint *constraint : constraint_list) {
                constraint->apply(obj, getStepDt());
            }
        }
    }

    void update_bodies(float dt) {
        for (auto &obj : body_list) {
            obj->update(dt);
        }
    }
 public:
    Solver() = default;
    Solver(Vec2 gravity): gravity{gravity} {}

    Body& addBody(Body* obj) {
        body_list.push_back(obj);
        return *obj;
    }

    void update() {
        m_time += m_frame_dt;
        const float step_dt = getStepDt();

        for (unsigned int i = sub_steps; i--;) {
            apply_gravity();
            check_collisions(step_dt);
            apply_constraints();
            update_bodies(step_dt);
        }
    }

    void setSimulationUpdateRate(uint32_t rate) {
        m_frame_dt = 1.0f / static_cast<float>(rate);
    }

    void addConstraint(Constraint* constraint) {
        constraint_list.push_back(constraint);
    }

    void setSubStepsCount(uint32_t sub_steps) {
        sub_steps = sub_steps;
    }

    void setObjectVelocity(Body &object, sf::Vector2f v) const {
        object.velocity = v;
    }

    [[nodiscard]]
    const std::vector<Body*> &getObjects() const {
        return body_list;
    }

    [[nodiscard]]
    const std::vector<Constraint*> &getConstraints() const {
        return constraint_list;
    }

    [[nodiscard]]
    uint64_t getObjectsCount() const {
        return body_list.size();
    }

    [[nodiscard]]
    float getTime() const {
        return m_time;
    }

    [[nodiscard]]
    float getStepDt() const {
        return m_frame_dt / static_cast<float>(sub_steps);
    }
};
