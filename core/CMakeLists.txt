cmake_minimum_required(VERSION 3.16)
project(pathfinder)
add_executable(core src/CYKonGraph.cpp)
add_subdirectory(../front/graph_utils/code ../front/graph_utils/code)