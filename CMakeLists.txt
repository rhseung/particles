cmake_minimum_required(VERSION 3.28)
project(particles)

set(CMAKE_CXX_STANDARD 20)

add_executable(particles
#        circle.cpp
#        performance_test.cpp
#        two_objects.cpp
        shape_test.cpp
)

# SFML
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)
target_link_libraries(particles sfml-graphics sfml-audio sfml-window sfml-system sfml-network)
