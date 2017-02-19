cmake_minimum_required(VERSION 3.5)

# Compile command in emacs for convenience

project (MTG)

# This makes cmake a little quiter
function(message)
  list(GET ARGV 0 MessageType)
  if(MessageType STREQUAL FATAL_ERROR OR
      MessageType STREQUAL SEND_ERROR OR
      MessageType STREQUAL WARNING OR
      MessageType STREQUAL AUTHOR_WARNING)
    list(REMOVE_AT ARGV 0)
    _message(${MessageType} "${ARGV}")
  endif()
  endfunction()
  
#add_subdirectory(SFML)
#add_subdirectory(entityx)
#add_subdirectory(googletest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")
include_directories(
#  ./entityx/entityx
  ./MTG
#  ./SFML
  ./Catch/single_include
  ./spdlog/include
  ./GSL/include
#  ./googletest/googletest/include
#  ./googletest/googlemock/include
)

add_executable(mtg main.cpp)
target_link_libraries(mtg LINK_PUBLIC entityx sfml-system sfml-window sfml-graphics)
set_target_properties(mtg PROPERTIES RULE_MESSAGES OFF)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif()

add_executable(testrunner
  test/testrunner.cpp
  test/startgame.cpp
  test/player.cpp
  test/card.cpp
  test/game.cpp
  )
set(THREADS_PREFER_PTHREAD_FLAG OFF)
SET_TARGET_PROPERTIES(testrunner PROPERTIES COMPILE_FLAGS "-Wall")
target_link_libraries(testrunner LINK_PUBLIC pthread entityx)
set_target_properties(testrunner PROPERTIES RULE_MESSAGES OFF)
