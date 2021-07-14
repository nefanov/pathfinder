cmake_minimum_required(VERSION 3.16)
project(pathfinder)

find_package(Catch2 QUIET)
if (TARGET Catch2::Catch2)
    include(Catch2)
else()
    message(not_found)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG devel
    )
    FetchContent_MakeAvailable(Catch2)
endif()

set(CMAKE_CXX_FLAGS "-std=c++17")
set(CMAKE_CXX_COMPILER g++-9)
set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(../core/include)
include_directories(_deps/catch2-src/src)

add_library(fst SHARED ../core/src/fst.cpp)
add_library(slw SHARED ../core/src/slw.cpp)
add_library(cre SHARED ../core/src/cre.cpp)
add_library(c2g SHARED gcc-cfg-utils/code/c2g.cpp)

add_executable(core ../core/src/core.cpp)
add_executable(code2graph gcc-cfg-utils/code/code2graph.cpp)
#add_executable(test tests/test.cpp)

target_link_libraries(core PRIVATE cre dl)
target_link_libraries(code2graph PRIVATE c2g)
#target_link_libraries(test PRIVATE c2g fst Catch2WithMain)