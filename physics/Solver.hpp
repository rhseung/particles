#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../utils/math.hpp"
#include "../engine/common/Constraints.hpp"
#include "../engine/common/Body.hpp"
#include "../engine/Collisions.hpp"

class Solver {
 private:
    Vec2 gravity;
    List<Body*> body_list;
    List<Constraint*> constraint_list;
    List<Manifold> manifolds;
    uint32_t sub_steps = 1;
    float time = 0.f;
    float frame_dt = 0.f;

    void applyGravity() {
        for (auto &obj : body_list) {
            if (!obj->isStatic())
                obj->accelerate(gravity);
        }
    }

    void resolveCollisions(float dt) {
        manifolds.clear();

        for (unsigned int i = 0; i < body_list.size(); i++) {
            for (unsigned int j = i + 1; j < body_list.size(); j++) {
                if (body_list[i]->isStatic() && body_list[j]->isStatic())
                    continue;

                if (body_list[i]->shape() == ShapeType::CIRCLE) {
                    auto obj1 = dynamic_cast<CircleBody*>(body_list[i]);

                    if (body_list[j]->shape() == ShapeType::CIRCLE) {
                        auto obj2 = dynamic_cast<CircleBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersectCircles(obj1->position(),
                                                         obj1->radius(),
                                                         obj2->position(),
                                                         obj2->radius(),
                                                         manifold)) {
                            float obj1_ratio = obj2->isStatic() ? 1.f : 1.f / (1.f + obj1->mass() / obj2->mass());
                            float obj2_ratio = obj1->isStatic() ? 1.f : 1.f / (1.f + obj2->mass() / obj1->mass());

                            obj1->move(-manifold.normal * obj1_ratio * manifold.depth);
                            obj2->move(manifold.normal * obj2_ratio * manifold.depth);

                            manifold.setBody(obj1, obj2);
                            manifolds.push_back(manifold);
                        }
                    }
                    else if (body_list[j]->shape() == ShapeType::POLYGON) {
                        auto obj2 = dynamic_cast<PolygonBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersectCircleAndPolygon(obj1->position(),
                                                                  obj1->radius(),
                                                                  obj2->position(),
                                                                  obj2->vertices(),
                                                                  manifold)) {
                            float obj1_ratio = obj2->isStatic() ? 1.f : 1.f / (1.f + obj1->mass() / obj2->mass());
                            float obj2_ratio = obj1->isStatic() ? 1.f : 1.f / (1.f + obj2->mass() / obj1->mass());

                            obj1->move(-manifold.normal * obj1_ratio * manifold.depth);
                            obj2->move(manifold.normal * obj2_ratio * manifold.depth);

                            manifold.setBody(obj1, obj2);
                            manifolds.push_back(manifold);
                        }
                    }
                }
                else if (body_list[i]->shape() == ShapeType::POLYGON) {
                    auto obj1 = dynamic_cast<PolygonBody*>(body_list[i]);

                    if (body_list[j]->shape() == ShapeType::POLYGON) {
                        auto obj2 = dynamic_cast<PolygonBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersectPolygons(obj1->position(),
                                                          obj1->vertices(),
                                                          obj2->position(),
                                                          obj2->vertices(),
                                                          manifold)) {
                            float obj1_ratio = obj2->isStatic() ? 1.f : 1.f / (1.f + obj1->mass() / obj2->mass());
                            float obj2_ratio = obj1->isStatic() ? 1.f : 1.f / (1.f + obj2->mass() / obj1->mass());

                            obj1->move(-manifold.normal * obj1_ratio * manifold.depth);
                            obj2->move(manifold.normal * obj2_ratio * manifold.depth);

                            manifold.setBody(obj1, obj2);
                            manifolds.push_back(manifold);
                        }
                    }
                    else if (body_list[j]->shape() == ShapeType::CIRCLE) {
                        auto obj2 = dynamic_cast<CircleBody*>(body_list[j]);

                        Manifold manifold;
                        if (Collisions::intersectCircleAndPolygon(obj2->position(),
                                                                  obj2->radius(),
                                                                  obj1->position(),
                                                                  obj1->vertices(),
                                                                  manifold)) {
                            float obj1_ratio = obj2->isStatic() ? 1.f : 1.f / (1.f + obj1->mass() / obj2->mass());
                            float obj2_ratio = obj1->isStatic() ? 1.f : 1.f / (1.f + obj2->mass() / obj1->mass());

                            obj2->move(-manifold.normal * obj2_ratio * manifold.depth);
                            obj1->move(manifold.normal * obj1_ratio * manifold.depth);

                            manifold.setBody(obj2, obj1);
                            manifolds.push_back(manifold);
                        }
                    }
                }
            }
        }

        for (auto &manifold : manifolds) {
            Collisions::resolveCollision(manifold);
        }
    }

    void applyConstraints() {
        for (uint32_t i = 4; i--;) {
            for (Constraint *constraint : constraint_list) {
                constraint->apply();
            }
        }
    }

    void updateBodies(float dt) {
        for (auto &obj : body_list) {
            obj->update(dt);
        }
    }

 public:
    Solver() = default;
    explicit Solver(Vec2 gravity, uint32_t sub_steps = 1, uint32_t fps = 120): gravity{gravity}, sub_steps{sub_steps}, frame_dt{1.0f / static_cast<float>(fps)} {}

    void update() {
        time += frame_dt;
        const float step_dt = getStepDt();

        for (unsigned int i = sub_steps; i--;) {
            applyGravity();
            resolveCollisions(step_dt);
            applyConstraints();
            updateBodies(step_dt);
        }
    }

    void setUpdateRate(uint32_t rate) {
        frame_dt = 1.0f / static_cast<float>(rate);
    }

    void setSubSteps(uint32_t steps) {
        this->sub_steps = steps;
    }

    [[nodiscard]]
    float getTime() const {
        return time;
    }

    [[nodiscard]]
    float getStepDt() const {
        return frame_dt / static_cast<float>(sub_steps);
    }

    [[nodiscard]]
    const List<Body*> &getBodyList() const {
        return body_list;
    }

    [[nodiscard]]
    const List<Constraint*> &getConstraintList() const {
        return constraint_list;
    }

    [[nodiscard]]
    const List<Manifold> &getManifolds() const {
        return manifolds;
    }

    [[nodiscard]]
    uint64_t getBodyCount() const {
        return body_list.size();
    }

    [[nodiscard]]
    uint64_t getConstraintCount() const {
        return constraint_list.size();
    }

    Body& addBody(Body* obj) {
        body_list.push_back(obj);
        return *obj;
    }

    bool removeBody(Body* obj) {
        auto it = std::find(body_list.begin(), body_list.end(), obj);
        if (it != body_list.end()) {
            body_list.erase(it);
            return true;
        }
        return false;
    }

    Body* getBody(uint32_t index) {
        return body_list[index];
    }

    void addConstraint(Constraint* constraint) {
        constraint_list.push_back(constraint);
    }

    bool removeConstraint(Constraint* constraint) {
        auto it = std::find(constraint_list.begin(), constraint_list.end(), constraint);
        if (it != constraint_list.end()) {
            constraint_list.erase(it);
            return true;
        }
        return false;
    }

    Constraint* getConstraint(uint32_t index) {
        return constraint_list[index];
    }

};
