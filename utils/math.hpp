#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct Math {
    static constexpr float PI = 3.1415936f;

    // normalize
    template <typename T>
    static sf::Vector2<T> normalize(const sf::Vector2<T> &v) {
        const float length = std::sqrt(v.x * v.x + v.y * v.y);
        return {v.x / length, v.y / length};
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

template <typename T>
T dot(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename T>
T cross(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
    return lhs.x * rhs.y - lhs.y * rhs.x;
}