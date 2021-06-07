#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "code2graph.h"
TEST_CASE("wrong path to the file") {
    std::ifstream input_file;
    REQUIRE(process_path(2, "./", input_file) == -1);
    
}

TEST_CASE("path to ../examples/test1.c") {
    std::ifstream input_file;
    REQUIRE(process_path(2, "../examples/test1.c", input_file) == 0);
}