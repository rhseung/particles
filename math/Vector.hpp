#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <format>
#include <concepts>
#include <cmath>

template <typename T> requires std::is_arithmetic_v<T>
class Vec {
 public:
    T x;
    T y;

    Vec() = default;
    Vec(T x, T y): x{x}, y{y} {}
    Vec(Vec<T> const &vec): x{static_cast<T>(vec.x)}, y{static_cast<T>(vec.y)} {}
    Vec(const sf::Vector2<T>& vec): x{vec.x}, y{vec.y} {}
    template <typename U> explicit Vec(Vec<U> vec): x{static_cast<T>(vec.x)}, y{static_cast<T>(vec.y)} {}

    friend Vec<T> operator+(const Vec<T>& lhs, const Vec<T>& rhs);
    friend Vec<T> operator-(const Vec<T>& lhs, const Vec<T>& rhs);
    friend Vec<T> operator*(const Vec<T>& lhs, T rhs);
    friend Vec<T> operator*(T lhs, const Vec<T>& rhs);
    friend T operator*(const Vec<T>& lhs, const Vec<T>& rhs);
    friend Vec<T> operator/(const Vec<T>& lhs, T rhs);
    friend Vec<T> operator/(T lhs, const Vec<T>& rhs);
    friend T operator^(const Vec<T>& lhs, const Vec<T>& rhs);

    Vec<T>& operator+=(const Vec<T>& rhs);
    Vec<T>& operator-=(const Vec<T>& rhs);
    Vec<T>& operator*=(T rhs);
    Vec<T>& operator/=(T rhs);

    T dot(const Vec<T>& rhs);
    T cross(const Vec<T>& rhs);

    Vec<T> normalize();
    float length();
    T lengthSquared();

    Vec<T> rotate(float angle, Vec<T> center);
    friend std::ostream& operator<<(std::ostream& os, const Vec<T>& vec);
};

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> Vec<T>::normalize() {
    float len = length();
    if (len == 0)
        len = 1;

    return {x / len, y / len};
}

template<typename T> requires std::is_arithmetic_v<T>
float Vec<T>::length() {
    return std::sqrt(lengthSquared());
}

template<typename T> requires std::is_arithmetic_v<T>
T Vec<T>::lengthSquared() {
    return x * x + y * y;
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator+(const Vec<T> &lhs, const Vec<T> &rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator-(const Vec<T> &lhs, const Vec<T> &rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator*(const Vec<T>& lhs, T rhs) {
    return {lhs.x * rhs, lhs.y * rhs};
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator*(T lhs, const Vec<T>& rhs) {
    return {rhs.x * lhs, rhs.y * lhs};
}

template <typename T> requires std::is_arithmetic_v<T>
T operator*(const Vec<T>& lhs, const Vec<T>& rhs) {
    return lhs.dot(rhs);
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator/(const Vec<T>& lhs, T rhs) {
    return {lhs.x / rhs, lhs.y / rhs};
}

template <typename T> requires std::is_arithmetic_v<T>
Vec<T> operator/(T lhs, const Vec<T>& rhs) {
    return {rhs.x / lhs, rhs.y / lhs};
}

template <typename T> requires std::is_arithmetic_v<T>
T operator^(const Vec<T>& lhs, const Vec<T>& rhs) {
    return lhs.cross(rhs);
}

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> &Vec<T>::operator+=(const Vec<T> &rhs) {
    return {x += rhs.x, y += rhs.y};
}

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> &Vec<T>::operator-=(const Vec<T> &rhs) {
    return {x -= rhs.x, y -= rhs.y};
}

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> &Vec<T>::operator*=(T rhs) {
    return {x *= rhs, y *= rhs};
}

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> &Vec<T>::operator/=(T rhs) {
    return {x /= rhs, y /= rhs};
}

template<typename T> requires std::is_arithmetic_v<T>
T Vec<T>::dot(const Vec<T> &rhs) {
    return x * rhs.x + y * rhs.y;
}

template<typename T> requires std::is_arithmetic_v<T>
T Vec<T>::cross(const Vec<T> &rhs) {
    return x * rhs.y - y * rhs.x;
}

template<typename T> requires std::is_arithmetic_v<T>
Vec<T> Vec<T>::rotate(float angle, Vec<T> center) {
    float s = std::sin(angle);
    float c = std::cos(angle);

    Vec ret = this - center;
    ret.x = ret.x * c - ret.y * s;
    ret.y = ret.x * s + ret.y * c;
    ret += center;

    return ret;
}

template<typename T> requires std::is_arithmetic_v<T>
std::ostream &operator<<(std::ostream &os, const Vec<T> &vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

template <typename T> requires std::is_arithmetic_v<T>
struct [[maybe_unused]] std::formatter<Vec<T>> : std::formatter<std::string> {
    auto format(const Vec<T>& v, format_context& ctx) {
        return std::formatter<std::string>::format(std::format("({}, {})", v.x, v.y), ctx);
    }
};

using IVec = Vec<int>;
using FVec = Vec<float>;