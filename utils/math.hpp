#pragma once
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <NumCpp.hpp>

struct Math {
    static constexpr float PI = std::numbers::pi_v<float>;

    template <typename T>
    static sf::Vector2<T> normalize(const sf::Vector2<T> v) {
        float length = std::sqrt(v.x * v.x + v.y * v.y);
        if (length == 0)
            length = 1;
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

    template <typename T>
    static float length(const sf::Vector2<T> &v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    template <typename T>
    static float lengthSquared(const sf::Vector2<T> &v) {
        return v.x * v.x + v.y * v.y;
    }

    template <typename T>
    static float angle(sf::Vector2<T> vec1, sf::Vector2<T> vec2) {
        return std::atan2(vec2.y - vec1.y, vec2.x - vec1.x);
    }

    template <typename T>
    static int ccw(sf::Vector2<T> a, sf::Vector2<T> b, sf::Vector2<T> c) {
        sf::Vector2<T> ab = b - a;
        sf::Vector2<T> ca = a - c;
        int64_t ccw = Math::cross(ab, ca);

        if (ccw > 0) return 1;
        if (ccw < 0) return -1;
        return 0;
    }

    template <typename T>
    static int ccw(sf::Vector2<T> a, sf::Vector2<T> b) {
        int64_t ccw = Math::cross(a, b);

        if (ccw > 0) return 1;
        if (ccw < 0) return -1;
        return 0;
    }
};

// ostream vector
template <typename T>
std::ostream& operator<<(std::ostream& os, sf::Vector2<T> v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

// std::formatter
template <typename T>
struct [[maybe_unused]] std::formatter<sf::Vector2<T>> : std::formatter<std::string> {
    auto format(const sf::Vector2<T>& v, format_context& ctx) {
        return std::formatter<std::string>::format(std::format("({}, {})", v.x, v.y), ctx);
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