cmake_minimum_required(VERSION 3.16)
project(pathfinder)

find_package(Catch2 QUIET)
if (TARGET Catch2::Catch2)
    include(Catch2)
else()
    message(catch2_not_found,_downloading...)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG devel
    )
    FetchContent_MakeAvailable(Catch2)
endif()

set(CMAKE_CXX_FLAGS "-std=c++17")
#set(CMAKE_CXX_COMPILER g++-9)
set(EXECUTABLE_OUTPUT_PATH build)
set(LIBRARY_OUTPUT_PATH build)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include_directories(../core/include)

add_library(fst SHARED ../core/src/fst.cpp)
add_library(slw SHARED ../core/src/slw.cpp)
add_library(cre SHARED ../core/src/cre.cpp)
add_library(c2g SHARED gcc-cfg-utils/code/c2g.cpp)
add_library(thin SHARED gcc-cfg-utils/code/w-thin-bb.cpp)
add_library(fat SHARED gcc-cfg-utils/code/wo-thin-bb.cpp)
add_library(orc SHARED gcc-cfg-utils/code/orchestrator.cpp)

add_executable(speed tests/speed_tests/test_speed.cpp)
add_executable(core ../core/src/core.cpp)
add_executable(code2graph gcc-cfg-utils/code/code2graph.cpp)
add_executable(test tests/test.cpp)

target_link_libraries(core PRIVATE cre dl)
target_link_libraries(code2graph PRIVATE c2g orc dl)
target_link_libraries(test PRIVATE c2g cre Catch2::Catch2WithMain orc dl)