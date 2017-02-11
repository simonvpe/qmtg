cmake_minimum_required(VERSION 3.5)

project (MTG)

add_subdirectory(SFML)
add_subdirectory(entityx)
add_subdirectory(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
include_directories(./entityx/entityx ./MTG ./SFML
  ./tinyfsm/include ./googletest/googletest/include)

add_executable(mtg main.cpp)
target_link_libraries(mtg LINK_PUBLIC entityx sfml-system sfml-window sfml-graphics)

add_executable(testrunner test/GideonAllyOfZendikar.cpp)
SET_TARGET_PROPERTIES(testrunner PROPERTIES COMPILE_FLAGS "-Wall")
target_link_libraries(testrunner LINK_PUBLIC entityx gtest gtest_main)

