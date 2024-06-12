//
// Created by rhseung on 24. 6. 10.
//

#ifndef PARTICLES_ENGINE_COMMON_RENDERABLE_HPP_
#define PARTICLES_ENGINE_COMMON_RENDERABLE_HPP_

#include <SFML/Graphics.hpp>
#include <vector>

class Renderable {
 public:
    virtual std::vector<sf::Drawable*> render() = 0;
};

#endif //PARTICLES_ENGINE_COMMON_RENDERABLE_HPP_
