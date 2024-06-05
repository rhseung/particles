#pragma once

#include "common/Body.hpp"
#include "../utils/math.hpp"

struct Manifold {
    Vec2 normal;
    float depth{};
};

class Collisions {
 public:
    static std::pair<float, float> projectPolygon(const List<Vec2> &vertices, Vec2 axis) {
        float min_a = axis * vertices[0];
        float max_a = min_a;

        for (const auto &vertex : vertices) {
            float proj = axis * vertex;
            min_a = std::min(min_a, proj);
            max_a = std::max(max_a, proj);
        }

        return {min_a, max_a};
    }

    static std::pair<float, float> projectCircle(Vec2 center, float radius, Vec2 axis) {
        Vec2 radius_vector = axis * radius;
        float p1 = (center + radius_vector) * axis;
        float p2 = (center - radius_vector) * axis;

        return {std::min(p1, p2), std::max(p1, p2)};
    }

    static void resolveCollision(Body& bodyA, Body& bodyB, Vec2 n, float depth) {
        Vec2 v_ab = bodyB.velocity - bodyA.velocity;
        if (v_ab * n > 0.f)
            return;

        float e = bodyA.material.restitution * bodyB.material.restitution;
        float inv_m_a = bodyA.inverseMass();
        float inv_m_b = bodyB.inverseMass();

        float j = -((1 + e) * v_ab * n) / (n * n * (inv_m_a + inv_m_b));

        bodyA.velocity = bodyA.velocity - (j * inv_m_a) * n;
        bodyB.velocity = bodyB.velocity + (j * inv_m_b) * n;
//        std::cout << j << "|" << bodyA.velocity << "|" << bodyB.velocity << std::endl;
    }

    static int findClosestPoint(Vec2 center, List<Vec2> vertices) {
        float min_dist = std::numeric_limits<float>::infinity();
        int closest = 0;

        for (int i = 0; i < vertices.size(); ++i) {
            float dist = std::abs(center - vertices[i]);
            if (dist < min_dist) {
                min_dist = dist;
                closest = i;
            }
        }

        return closest;
    }

    static bool intersectCircles(Vec2 center_a, float radius_a, Vec2 center_b, float radius_b, Manifold &out) {
        float dist = std::abs(center_a - center_b);
        if (dist >= radius_a + radius_b)
            return false;

        out.normal = Math::normalize(center_b - center_a);
        out.depth = (radius_a + radius_b) - dist;
        return true;
    }

    static bool intersectPolygons(Vec2 center_a, List<Vec2> vertices_a, Vec2 center_b, List<Vec2> vertices_b, Manifold &out) {
        out.normal = Vec2{};
        out.depth = std::numeric_limits<float>::infinity();

        for (int i = 0; i < vertices_a.size(); ++i) {
            Vec2 v_a = vertices_a[i];
            Vec2 v_b = vertices_a[(i + 1) % vertices_a.size()];

            Vec2 edge = v_b - v_a;
            Vec2 axis = Math::normalize(Vec2{edge.y, -edge.x});

            // min/max projection for A
            auto [min_a, max_a] = projectPolygon(vertices_a, axis);

            // min/max projection for B
            auto [min_b, max_b] = projectPolygon(vertices_b, axis);

            if (max_b <= min_a || max_a <= min_b)
                return false;

            float overlap = std::min(max_b - min_a, max_a - min_b);
            if (overlap < out.depth) {
                out.depth = overlap;
                out.normal = axis;
            }
        }

        for (int i = 0; i < vertices_b.size(); ++i) {
            Vec2 v_a = vertices_b[i];
            Vec2 v_b = vertices_b[(i + 1) % vertices_b.size()];

            Vec2 edge = v_b - v_a;
            Vec2 axis = Math::normalize(Vec2{edge.y, -edge.x});

            // min/max projection for A
            auto [min_a, max_a] = projectPolygon(vertices_a, axis);

            // min/max projection for B
            auto [min_b, max_b] = projectPolygon(vertices_b, axis);

            if (max_b <= min_a || max_a <= min_b)
                return false;

            float overlap = std::min(max_b - min_a, max_a - min_b);
            if (overlap < out.depth) {
                out.depth = overlap;
                out.normal = axis;
            }
        }

        Vec2 direction = center_b - center_a;
        if (direction * out.normal < 0)
            out.normal = -out.normal;

        return true;
    }

    static bool intersectCircleAndPolygon(Vec2 center_circle, float radius, Vec2 center_poly, List<Vec2> vertices, Manifold &out) {
        out.normal = Vec2{};
        out.depth = std::numeric_limits<float>::infinity();

        for (int i = 0; i < vertices.size(); ++i) {
            Vec2 v_a = vertices[i];
            Vec2 v_b = vertices[(i + 1) % vertices.size()];
            Vec2 edge = v_b - v_a;
            Vec2 axis = Math::normalize(Vec2{edge.y, -edge.x});

            // min/max projection for poly
            auto [min_p, max_p] = projectPolygon(vertices, axis);

            // min/max projection for circle
            auto [min_c, max_c] = projectCircle(center_circle, radius, axis);

            if (max_c <= min_p || max_p <= min_c)
                return false;

            float overlap = std::min(max_c - min_p, max_p - min_c);
            if (overlap < out.depth) {
                out.depth = overlap;
                out.normal = axis;
            }
        }

        int closest = findClosestPoint(center_circle, vertices);
        Vec2 closest_vertex = vertices[closest];
        Vec2 axis = Math::normalize(closest_vertex - center_circle);

        // min/max projection for poly
        auto [min_p, max_p] = projectPolygon(vertices, axis);

        // min/max projection for circle
        auto [min_c, max_c] = projectCircle(center_circle, radius, axis);

        if (max_c <= min_p || max_p <= min_c)
            return false;

        float overlap = std::min(max_c - min_p, max_p - min_c);
        if (overlap < out.depth) {
            out.depth = overlap;
            out.normal = axis;
        }

        Vec2 direction = center_poly - center_circle;
        if (direction * out.normal < 0)
            out.normal = -out.normal;

        return true;
    }
};
