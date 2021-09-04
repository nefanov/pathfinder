#define CATCH_CONFIG_MAIN
#include "catch2/catch_test_macros.hpp"
#include "code2graph.h"
#include "orchestrator.h"
#include "core.h"
#include <set>
TEST_CASE("wrong path to the file") {
    char* argv[] = {const_cast<char*>("test"), const_cast<char*>("-cmd"), const_cast<char*>("./")};
    options opt(3, argv);
    std::ifstream input_file, analyze_file;
    REQUIRE(process_path(opt, input_file, analyze_file) == -1);
}

TEST_CASE("using file input/test1.in") {
    std::ifstream input_file, analyze_file;
    char* argv[] = {const_cast<char*>("test"), const_cast<char*>("-file"), const_cast<char*>("tests/test1.in")};
    options opt(3, argv);
    REQUIRE(process_path(opt, input_file, analyze_file) == 0);
}

TEST_CASE("path to examples/test1.c") {
    std::ifstream input_file, analyze_file;
    char* argv [] = {const_cast<char*>("test"), const_cast<char*>("-cmd"), const_cast<char*>("gcc-cfg-utils/examples/test1.c")};
    options opt(3, argv);
    REQUIRE(process_path(opt, input_file, analyze_file) == 0);
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

TEST_CASE("test_front") {
    std::ifstream test_list("tests/test_front.txt");
    std::string test_input_name, test_graph_name, str1, str2;
    mkdir("data", 0777);
    while(!test_list.eof()) {
        test_list >> test_input_name >> test_graph_name;
        system(("build/code2graph -file tests/" + test_input_name + " -front-only > data/graph.test").c_str());
        std::ifstream graph("data/graph");
        std::ifstream testgraph("tests/" + test_graph_name);
        while(!graph.eof() && !testgraph.eof()) {
            graph >> str1;
            testgraph >> str2;
            REQUIRE(str1 == str2);
        }
        graph.close();
        testgraph.close();
    }
    test_list.close();
}


TEST_CASE("test_core fast") {
    std::ifstream test_list("tests/test_core.txt");
    std::string test_graph_name, test_output_name, type, str1, str2;
    std::multiset<std::pair<std::string, std::string>> s1, s2;
    std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> t;

    mkdir("data", 0777);
    while(!test_list.eof()) {
        /*test_list >> type;
        if (type == "slow")
            continue;*/
        test_list >> test_graph_name >> test_output_name;
        std::cout << "build/core tests/" + test_graph_name + " -fast > data/output.test\n";
        system(("build/core tests/" + test_graph_name + " -fast > data/output.test").c_str());
        std::ifstream output("data/output.test");
        std::ifstream testoutput("tests/" + test_output_name);
        REQUIRE(output.is_open());
        REQUIRE(testoutput.is_open());
        std::getline(output, type);
        REQUIRE(type == "libfst.so");
        while(!output.eof() && !testoutput.eof()) {
            std::getline(output, str1);
            std::getline(output, str2);
            s1.insert({str1, str2});
            std::getline(testoutput, str1);
            std::getline(testoutput, str2);
            s2.insert({str1, str2});
        }
        REQUIRE(s1 == s2);
        output.close();
        testoutput.close();
    }
    test_list.close();
}


TEST_CASE("test_core slow") {
    std::ifstream test_list("tests/test_core.txt");
    std::string test_graph_name, test_output_name, type, str1, str2;
    std::multiset<std::pair<std::string, std::string>> s1, s2;
    std::pair<std::pair<std::string, std::string>, std::pair<std::string, std::string>> t;

    mkdir("data", 0777);
    while(!test_list.eof()) {
        /*test_list >> type;
        if (type == "fast")
            continue;*/
        test_list >> test_graph_name >> test_output_name;
        std::cout << "build/core tests/" + test_graph_name + " -slow > data/output.test\n";
        system(("build/core tests/" + test_graph_name + " -slow > data/output.test").c_str());
        std::ifstream output("data/output.test");
        std::ifstream testoutput("tests/" + test_output_name);
        REQUIRE(output.is_open());
        REQUIRE(testoutput.is_open());
        std::getline(output, type);
        REQUIRE(type == "libslw.so");
        while(!output.eof() && !testoutput.eof()) {
            std::getline(output, str1);
            std::getline(output, str2);
            s1.insert({str1, str2});
            std::getline(testoutput, str1);
            std::getline(testoutput, str2);
            s2.insert({str1, str2});
        }
        REQUIRE(s1 == s2);
        output.close();
        testoutput.close();
    }
    test_list.close();
}