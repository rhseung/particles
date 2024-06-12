#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <numbers>
#include "math.hpp"

sf::Color getRainbow(float t) {
    const float r = std::sin(t);
    const float g = std::sin(t + 0.33f * 2.0f * Math::PI);
    const float b = std::sin(t + 0.66f * 2.0f * Math::PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

sf::Color whiteOrBlack(const sf::Color &color) {
    return (color.r + color.g + color.b) / 3 > 128 ? sf::Color::Black : sf::Color::White;
}
