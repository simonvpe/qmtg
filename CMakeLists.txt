cmake_minimum_required(VERSION 3.5)

project (MTG)

add_subdirectory(SFML)
add_subdirectory(entityx)

add_executable(mtg main.cpp)
set_property(TARGET mtg PROPERTY CXX_STANDARD 14)
target_link_libraries(mtg LINK_PUBLIC entityx sfml-system sfml-window sfml-graphics)
include_directories(./entityx/entityx ./MTG ./SFML ./tinyfsm/include)

