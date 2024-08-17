cmake_minimum_required(VERSION 3.28)
project(particles)
set(CMAKE_CXX_STANDARD 20)

add_executable(particles
        examples/circle.cpp
        math/Vector.hpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_ROOT "D:/tools/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS REQUIRED graphics audio window system network)
include_directories("D/tools/SFML-2.6.1/include")
target_link_libraries(particles sfml-graphics sfml-audio sfml-window sfml-system sfml-network)

target_include_directories(particles PRIVATE utils engine renderer physics)
