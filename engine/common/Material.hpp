#pragma once

#include <SFML/Graphics.hpp>

class Material {
 public:
    const float density;
    const float restitution;
    const float us;   // static friction
    const float uk;   // kinetic friction
    const sf::Color color;

    Material(float density, float restitution, float us, float uk, sf::Color color)
        : density{density}, restitution{restitution}, us{us}, uk{uk}, color{color} {}
};

namespace Materials {
const Material wood{0.6f, 0.4f, 0.6f, 0.4f, sf::Color{139, 69, 19}};
const Material rubber{1.5f, 0.9f, 1.0f, 0.8f, sf::Color{255, 69, 0}};
const Material metal{2.4f, 0.1f, 1.0f, 0.8f, sf::Color{192, 192, 192}};
const Material ice{0.9f, 0.1f, 0.1f, 0.05f, sf::Color{135, 206, 250}};
const Material glass{2.6f, 0.1f, 0.4f, 0.15f, sf::Color{0, 191, 255}};
const Material stone{2.0f, 0.1f, 0.6f, 0.4f, sf::Color{128, 128, 128}};
const Material sand{1.5f, 0.1f, 0.6f, 0.4f, sf::Color{244, 164, 96}};
const Material paper{0.8f, 0.4f, 0.6f, 0.4f, sf::Color{255, 228, 196}};
const Material plastic{1.0f, 0.4f, 0.6f, 0.4f, sf::Color{255, 222, 173}};
const Material diamond{3.5f, 0.1f, 0.1f, 0.05f, sf::Color{185, 242, 255}};
const Material gold{19.3f, 0.1f, 0.1f, 0.05f, sf::Color{255, 215, 0}};
const Material silver{10.5f, 0.1f, 0.1f, 0.05f, sf::Color{192, 192, 192}};
const Material copper{8.9f, 0.1f, 0.1f, 0.05f, sf::Color{ 184, 115, 51 }};
const Material ideal{1.0f, 1.0f, 0.0f, 0.0f, sf::Color::White};
};