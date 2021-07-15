#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "code2graph.h"
#include "core.h"
TEST_CASE("wrong path to the file") {
    std::ifstream input_file, analyze_file;
    std::string path = "./", path_to_input = "";
    REQUIRE(process_path(2, input_file, path, path_to_input, analyze_file) == -1);
}

TEST_CASE("using file input/test1.in") {
    std::ifstream input_file, analyze_file;
    std::string path_to_input = "tests/test1.in", path = "", full_path = std::filesystem::current_path().string() + "/" + path_to_input;
    REQUIRE(process_path(3, input_file, path, path_to_input, analyze_file) == 0);
    REQUIRE(path_to_input == full_path);
    full_path.erase(full_path.find_last_of("/") + 1, full_path.size()); // .../gcc-cfg-utils/input/test.in -> .../gcc-cfg-utils/input/
    REQUIRE(path == "gcc-cfg-utils/examples/test1.c.012t.cfg.dot");
}

TEST_CASE("path to examples/test1.c") {
    std::ifstream input_file, analyze_file;
    std::string path = "gcc-cfg-utils/examples/test1.c", path_to_input = "";
    REQUIRE(process_path(2, input_file, path, path_to_input, analyze_file) == 0);
}

TEST_CASE("new_fastset") {
    void* sl = dlopen("libfst.so", RTLD_LAZY);
    REQUIRE(sl != NULL);
    void* f = dlsym(sl, "new_fastset");
    REQUIRE(f != NULL);
    std::vector<unsigned int> (*new_fastset)(int P, int V) = reinterpret_cast<std::vector<unsigned int> (*)(int P, int V)>(f);
    std::vector<unsigned int> check1(4, 0), check2(3, 0);
    REQUIRE(check1 == new_fastset(3, 10));
    REQUIRE(check2 == new_fastset(4, 12));
}

TEST_CASE("difference") {
    void* sl = dlopen("libfst.so", RTLD_LAZY);
    REQUIRE(sl != NULL);
    void* f = dlsym(sl, "difference");
    REQUIRE(f != NULL);
    std::vector<unsigned int> (*difference)(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2) = reinterpret_cast<std::vector<unsigned int> (*)(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)>(f);
    int P = 1, V = 5;
    std::vector<unsigned int> v11 = {1, 0, 1, 0, 1}, v12 = {0, 1, 1, 0, 0}, check1 = {1, 0, 0, 0, 1};
    REQUIRE(check1 == difference(P, V, v11, v12));
    P = 2, V = 6;
    std::vector<unsigned int> v21 = {3, 2, 1}, v22 = {2, 1, 1}, check2 = {1, 2, 0};
    REQUIRE(check2 == difference(P, V, v21, v22));
}

TEST_CASE("add_value fast") {
    void* sl = dlopen("libfst.so", RTLD_LAZY);
    REQUIRE(sl != NULL);
    void* f = dlsym(sl, "add_value");
    REQUIRE(f != NULL);
    void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P) = reinterpret_cast<void (*)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P)>(f);
    int a1 = 2, a2 = 3, P = 2;
    std::vector<unsigned int> v1 = {3, 2, 1}, v2 = v1, check1 = {3, 3, 1}, check2 = {3, 2, 1};
    std::unordered_set<int> u;
    add_value(u, v1, a1, P);
    add_value(u, v2, a2, P);
    REQUIRE(check1 == v1);
    REQUIRE(check2 == v2);
}

TEST_CASE("add_value slow") {
    void* sl = dlopen("libslw.so", RTLD_LAZY);
    REQUIRE(sl != NULL);
    void* f = dlsym(sl, "add_value");
    REQUIRE(f != NULL);
    void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P) = reinterpret_cast<void (*)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P)>(f);
    int a1 = 2, a2 = 3, P = 2;
    std::vector<unsigned int> v1 = {3, 2, 1}, v2 = v1, check1 = {3, 3, 1}, check2 = {3, 2, 1};
    std::unordered_set<int> u1 = {3, 2, 1}, u2 = {2, 4, 1}, check1u = {1, 2, 3}, check2u = {3, 2, 4, 1};
    add_value(u1, v1, a1, P);
    add_value(u2, v2, a2, P);
    REQUIRE(check1u == u1);
    REQUIRE(check2u == u2);
}

TEST_CASE("not_null") {
    void* sl = dlopen("libfst.so", RTLD_LAZY);
    REQUIRE(sl != NULL);
    void* f = dlsym(sl, "not_null");
    REQUIRE(f != NULL);
    std::vector<int> (*not_null)(int P, std::vector<unsigned int> kek) = reinterpret_cast<std::vector<int> (*)(int P, std::vector<unsigned int> kek)>(f);
    int P1 = 2, P2= 3;
    std::vector<unsigned int> v1 = {3, 1, 2, 0}, v2 = {0, 1, 0, 5};
    std::vector<int> check1 = {0, 1, 2, 5}, check2 = {3, 9, 11};
    REQUIRE(check1 == not_null(P1, v1));
    REQUIRE(check2 == not_null(P2, v2));
}

TEST_CASE("path_find") {
    int i = 0, j = 1, nonterm = 0;
    std::vector<std::vector<std::vector<std::vector<int> > > > prev(3, std::vector<std::vector<std::vector<int>>>(4, std::vector<std::vector<int>>(3, std::vector<int>(3, -1))));
    prev[0][0][1][0] = 2, prev[0][0][1][1] = 2, prev[0][0][1][2] = 1;
    std::vector<int> check = {0 , 2, 1};
    REQUIRE(check == path_find(i, j, nonterm, prev));
}

TEST_CASE("check") {
    int initial = 0;
    std::vector<std::string> a = {"A", "S", "B"}, a3 = {"A", "B", "S"};
    std::string b1 = "A", b2 = "R", b3 = "S";
    int check1 = 0, check2 = 3, check3 = 2; 
    REQUIRE(check1 == check(initial, a, b1));
    REQUIRE(initial == 0);
    REQUIRE(check2 == check(initial, a, b2));
    REQUIRE(initial == 0);
    REQUIRE(check3 == check(initial, a3, b3));
    REQUIRE(initial == 2);
}

TEST_CASE("test1.in") {
    system("build/code2graph -file tests/test1.in -front-only");
    std::ifstream graph("data/graph");
    std::ifstream testgraph("tests/test_graph1");
    std::string str1, str2;
    while(!graph.eof() && !testgraph.eof()) {
        graph >> str1;
        testgraph >> str2;
        REQUIRE(str1 == str2);
    }
    graph.close();
    testgraph.close();
}

TEST_CASE("test2.in") {
    system("build/code2graph -file tests/test2.in -front-only");
    std::ifstream graph("data/graph");
    std::ifstream testgraph("tests/test_graph2");
    std::string str1, str2;
    while(!graph.eof() && !testgraph.eof()) {
        graph >> str1;
        testgraph >> str2;
        REQUIRE(str1 == str2);
    }
    graph.close();
    testgraph.close();
}