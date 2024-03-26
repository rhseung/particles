cmake_minimum_required(VERSION 3.28)
project(particles)

set(CMAKE_CXX_STANDARD 20)

add_executable(particles main.cpp)
add_subdirectory(libs/glfw-3.4)
target_link_libraries(particles glfw)
