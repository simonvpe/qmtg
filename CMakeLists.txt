cmake_minimum_required(VERSION 3.5)

project (MTG)

add_subdirectory(SFML)
add_subdirectory(entityx)

add_executable(mtg main.cpp)
target_link_libraries(mtg LINK_PUBLIC entityx sfml-system sfml-window)
include_directories(./entityx/entityx ./MTG ./SFML)

