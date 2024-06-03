#pragma once

#include "common/Body.hpp"
#include "../utils/math.hpp"

struct Manifold {
    Vec2 normal;
    float depth;
};

class Collisions {
 public:
    static bool intersect_circles(Vec2 center_a, float radius_a, Vec2 center_b, float radius_b, Manifold &out) {
        float dist = std::abs(center_a - center_b);
        if (dist >= radius_a + radius_b)
            return false;

        out.normal = Math::normalize(center_b - center_a);
        out.depth = (radius_a + radius_b) - dist;
        return true;
    }

    static bool intersect_polys(Vec2 center_a, List<Vec2> vertices_a, Vec2 center_b, List<Vec2> vertices_b, Manifold &out) {
        out.normal = Vec2{};
        out.depth = std::numeric_limits<float>::infinity();

        for (int i = 0; i < vertices_a.size(); ++i) {
            Vec2 v_a = vertices_a[i];
            Vec2 v_b = vertices_a[(i + 1) % vertices_a.size()];

            Vec2 edge = v_b - v_a;
            Vec2 axis = {edge.y, -edge.x};

            // min/max projection for A
            float min_a = axis * vertices_a[0];
            float max_a = min_a;
            for (const auto &vertex : vertices_a) {
                float proj = axis * vertex;
                min_a = std::min(min_a, proj);
                max_a = std::max(max_a, proj);
            }

            // min/max projection for B
            float min_b = axis * vertices_b[0];
            float max_b = min_b;
            for (const auto &vertex : vertices_b) {
                float proj = axis * vertex;
                min_b = std::min(min_b, proj);
                max_b = std::max(max_b, proj);
            }

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
            Vec2 axis = {edge.y, -edge.x};

            // min/max projection for A
            float min_a = axis * vertices_a[0];
            float max_a = min_a;
            for (const auto &vertex : vertices_a) {
                float proj = axis * vertex;
                min_a = std::min(min_a, proj);
                max_a = std::max(max_a, proj);
            }

            // min/max projection for B
            float min_b = axis * vertices_b[0];
            float max_b = min_b;
            for (const auto &vertex : vertices_b) {
                float proj = axis * vertex;
                min_b = std::min(min_b, proj);
                max_b = std::max(max_b, proj);
            }

            if (max_b <= min_a || max_a <= min_b)
                return false;

            float overlap = std::min(max_b - min_a, max_a - min_b);
            if (overlap < out.depth) {
                out.depth = overlap;
                out.normal = axis;
            }
        }

        out.depth /= std::abs(out.normal);
        out.normal = Math::normalize(out.normal);

        Vec2 direction = center_b - center_a;
        if (direction * out.normal < 0)
            out.normal = -out.normal;

        return true;
    }
};
