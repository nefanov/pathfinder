cmake_minimum_required(VERSION 3.16)
project(pathfinder)

set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CXX_COMPILER g++-9)
set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)

include_directories(../../core/include)

add_library(cyk SHARED ../../core/src/functions.cpp)
add_library(c2g SHARED code/funcs.cpp)

add_executable(core ../../core/src/CYKonGraph.cpp)
add_executable(code2graph code/code2graph.cpp)
add_executable(test ../../tests/test.cpp)

target_link_libraries(core PRIVATE cyk)
target_link_libraries(code2graph PRIVATE c2g)
target_link_libraries(test PRIVATE c2g)