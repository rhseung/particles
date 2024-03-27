cmake_minimum_required(VERSION 3.28)
project(particles)

set(CMAKE_CXX_STANDARD 20)

add_executable(particles main.cpp
        libs/object.hpp libs/renderer.hpp)
add_subdirectory(libs/SFML-2.6.1)
target_link_libraries(particles sfml-graphics sfml-window sfml-system)
