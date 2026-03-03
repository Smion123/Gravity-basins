cmake_minimum_required(VERSION 3.16)
project(gravity_basins)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Ofast -ffast-math -g -Wall -Werror)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

include_directories(include)

add_executable(gravity_basins
    src/main.cpp
    src/Planet.cpp
    src/StaticPlanet.cpp
    src/ForceCalculation.cpp
)

target_link_options(gravity_basins PRIVATE)

target_link_libraries(gravity_basins
    sfml-graphics
    sfml-window
    sfml-system
)
