//
// Created by rhseung on 2024-03-27.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "object.hpp"

namespace pt {
    class Renderer {
    public:
        sf::RenderWindow& window;
        std::vector<Circle> objects;

        explicit Renderer(sf::RenderWindow& window): window(window) {}

        void add(const Circle &obj) {
            objects.push_back(obj);
        }

        void render() {
            for (auto& obj : objects) {
                window.draw(obj.draw());
            }
        }
    };
}
