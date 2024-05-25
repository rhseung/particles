#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>

struct Math {
    static constexpr float PI = 3.1415936f;
};

namespace std {
    template <typename T>
    static float abs(const sf::Vector2<T> &v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }
}

template <typename T>
inline sf::Vector2<T> operator*(const sf::Vector2<T> &lhs, const sf::Vector2<T> &rhs) {
    return {lhs.x * rhs.x, lhs.y * rhs.y};
}