//
// Created by rhseung on 2024-03-27.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "object.hpp"

namespace pt {
class Renderer {
 public:
    sf::RenderWindow &window;
    std::vector<Object*> objects;

    explicit Renderer(sf::RenderWindow &window) : window(window) {}

    void add(Object* obj) {
        objects.push_back(obj);
    }

    void addMany(std::initializer_list<Object*> obj_list) {
        for (auto obj : obj_list) {
            objects.push_back(obj);
        }
    }

    void render() {
        for (auto &obj : objects) {
            window.draw(*(obj->render()));
        }
    }
};
}
