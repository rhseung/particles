#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct Math {
    static constexpr float PI = 3.1415936f;

    template <typename T>
    static sf::Vector2<T> normalize(const sf::Vector2<T> v) {
        const float length = std::sqrt(v.x * v.x + v.y * v.y);
        return {v.x / length, v.y / length};
    }

    template <typename T>
    static T dot(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename T>
    static T cross(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    static sf::Vector2<float> &rotate(sf::Vector2<float> &vertex, float angle, sf::Vector2<float> center) {
        const float s = std::sin(angle);
        const float c = std::cos(angle);

        vertex -= center;
        vertex = {
            vertex.x * c - vertex.y * s,
            vertex.x * s + vertex.y * c
        };
        vertex += center;

        return vertex;
    }
};

namespace std {
    template <typename T>
    static float abs(const sf::Vector2<T> &v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }
}

template <typename T>
inline T operator*(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}