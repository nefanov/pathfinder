#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "code2graph.h"
#include "CYKonGraph.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

TEST_CASE("wrong path to the file") {
    std::ifstream input_file, analyze_file;
    std::string path = "./", path_to_input = "";
    REQUIRE(process_path(2, input_file, path, path_to_input, analyze_file, 0) == -1);
}

TEST_CASE("using file input/test1.in") {
    std::ifstream input_file, analyze_file;
    std::string path_to_input = "", path = "tests/test1.in", full_path = std::filesystem::current_path().string() + "/" + path;
    REQUIRE(process_path(3, input_file, path, path_to_input, analyze_file, 1) == 0);
    REQUIRE(path_to_input == full_path);
    full_path.erase(full_path.find_last_of("/") + 1, full_path.size()); // .../gcc-cfg-utils/input/test.in -> .../gcc-cfg-utils/input/
    REQUIRE(path == full_path + "../examples/test1.c" + ".012t.cfg.dot");
}

TEST_CASE("path to examples/test1.c") {
    std::ifstream input_file, analyze_file;
    std::string path = "examples/test1.c", path_to_input = "";
    REQUIRE(process_path(2, input_file, path, path_to_input, analyze_file, 0) == 0);
}

TEST_CASE("examples/test1.c.012t.cfg.dot") {
    std::string inp, code = "";
    int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
    std::ifstream analyze_file("examples/test1.c.012t.cfg.dot");
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::string>> V, Code;
    std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
    while(std::getline(analyze_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0)
			blocks_handler(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, len);
		else
			code_handler(inp, basic_block, code, Code, subgraph);
	}
    REQUIRE(Clusters[0].first == "foo");
    REQUIRE(Clusters[1].first == "main");
    REQUIRE(Clusters[0].second.first == 0);
    REQUIRE(Clusters[0].second.second == 1);
    REQUIRE(Clusters[1].second.first == 0);
    REQUIRE(Clusters[1].second.second == 1);
    REQUIRE(V[0][0] == "fn_0_basic_block_0");
    REQUIRE(Code[0][0] == "ENTRY\n");
    REQUIRE(V[0][1] == "fn_0_basic_block_1");
    REQUIRE(Code[0][1] == "EXIT\n");
    REQUIRE(V[0][2] == "fn_0_basic_block_2");
    REQUIRE(V[1][0] == "fn_1_basic_block_0");
    REQUIRE(Code[0][0] == "ENTRY\n");
    REQUIRE(V[1][1] == "fn_1_basic_block_1");
    REQUIRE(Code[0][1] == "EXIT\n");
    REQUIRE(V[1][2] == "fn_1_basic_block_2");
    REQUIRE(V[1][3] == "fn_1_basic_block_3");

    REQUIRE(V[0][E[0][0][0].first] == "fn_0_basic_block_2");
    REQUIRE(V[0][E[0][0][1].first] == "fn_0_basic_block_1");
    REQUIRE(V[0][E[0][2][0].first] == "fn_0_basic_block_1");

    REQUIRE(V[1][E[1][0][0].first] == "fn_1_basic_block_2");
    REQUIRE(V[1][E[1][0][1].first] == "fn_1_basic_block_1");
    REQUIRE(V[1][E[1][2][0].first] == "fn_1_basic_block_3");
    REQUIRE(V[1][E[1][3][0].first] == "fn_1_basic_block_1");
}

TEST_CASE("test_graph1") {
    int initial, tests;
    std::ifstream fin(std::filesystem::current_path().string() + "/tests/test_graph1");
    REQUIRE(fin.is_open());
    fin >> tests;
    for (int test = 0, m, V, E; test < tests; test++) {
        std::vector<rule> rules;
        std::vector<int> topsort, way;
        std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
        std::vector<std::string> nonterminals;
        std::vector<std::vector<int> > RTDG;
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;
        fin >> m; //quantity of rules
        for (int i = 0; i < m; i++)
            input_rules(i, nonterminals, rules, fin, initial);
        fin >> V >> E;
        std::vector<std::pair<int, std::pair<int, std::string> > > edges;
        std::vector <std::string> V_names(V);
        std::vector<std::vector<std::pair<int, std::pair<int, int> > > > l (V, std::vector<std::pair<int, std::pair<int, int> > > (V, {-1, {-1, -1}}));
        input_V_names(V_names, edges, fin, V, E);
        for (int i = 0; i < nonterminals.size(); i++)
            last.push_back(l);
        find_rtdg(RTDG, nonterminals, topsort, rules);
        // Semi-Naive CFL
        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
            arranging_rules_to_edges(i, eps, edges, g, g_l, initial, last, rules , V);
        std::vector<std::vector<std::vector<int> > > delta = g;
        for (int k = RTDG.size() - 1, flag = 1; k >= 0; k--, flag = 1) //
            for (flag; flag; flag = 0)
                for (auto i : RTDG[k])
                    if (not_null(delta[i]))
                        transitive_closure(flag, delta, RTDG, g, i, k, last, rules);
        if (test == 0) {
            REQUIRE(count_ans(g, initial) == 2);
            REQUIRE(g[initial][0][1] == 1);
            way = path_find(g_l, 0, 1, initial, last);
            REQUIRE(way.size() == 2);
            REQUIRE(V_names[way[0]] == "fn_0_basic_block_0");
            REQUIRE(V_names[way[1]] == "fn_0_basic_block_1");
            REQUIRE(g[initial][0][2] == 1);
            way = path_find(g_l, 0, 2, initial, last);
            REQUIRE(way.size() == 2);
            REQUIRE(V_names[way[0]] == "fn_0_basic_block_0");
            REQUIRE(V_names[way[1]] == "fn_0_basic_block_2");
        }
        else {
            REQUIRE(count_ans(g, initial) == 2);
            REQUIRE(g[initial][2][3] == 1);
            REQUIRE(g[initial][3][1] == 1);
            way = path_find(g_l, 2, 3, initial, last);
            REQUIRE(way.size() == 2);
            REQUIRE(V_names[way[0]] == "fn_1_basic_block_2");
            REQUIRE(V_names[way[1]] == "fn_1_basic_block_3");
            way = path_find(g_l, 3, 1, initial, last);
            REQUIRE(way.size() == 2);
            REQUIRE(V_names[way[0]] == "fn_1_basic_block_3");
            REQUIRE(V_names[way[1]] == "fn_1_basic_block_1");
        }
    }
}

TEST_CASE("test_graph2") {
    int initial, tests;
    std::ifstream fin(std::filesystem::current_path().string() + "/tests/test_graph2");
    REQUIRE(fin.is_open());
    fin >> tests;
    for (int test = 0, m, V, E; test < tests; test++) {
        std::vector<rule> rules;
        std::vector<int> topsort, way;
        std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
        std::vector<std::string> nonterminals;
        std::vector<std::vector<int> > RTDG;
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;
        fin >> m; //quantity of rules
        for (int i = 0; i < m; i++)
            input_rules(i, nonterminals, rules, fin, initial);
        fin >> V >> E;
        std::vector<std::pair<int, std::pair<int, std::string> > > edges;
        std::vector <std::string> V_names(V);
        std::vector<std::vector<std::pair<int, std::pair<int, int> > > > l (V, std::vector<std::pair<int, std::pair<int, int> > > (V, {-1, {-1, -1}}));
        input_V_names(V_names, edges, fin, V, E);
        for (int i = 0; i < nonterminals.size(); i++)
            last.push_back(l);
        find_rtdg(RTDG, nonterminals, topsort, rules);
        // Semi-Naive CFL
        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
            arranging_rules_to_edges(i, eps, edges, g, g_l, initial, last, rules , V);
        std::vector<std::vector<std::vector<int> > > delta = g;
        for (int k = RTDG.size() - 1, flag = 1; k >= 0; k--, flag = 1) //
            for (flag; flag; flag = 0)
                for (auto i : RTDG[k])
                    if (not_null(delta[i]))
                        transitive_closure(flag, delta, RTDG, g, i, k, last, rules);
        if (test == 0) {
            REQUIRE(count_ans(g, initial) == 1);
            REQUIRE(g[initial][0][1] == 1);
            way = path_find(g_l, 0, 1, initial, last);
            REQUIRE(way.size() == 3);
            REQUIRE(V_names[way[0]] == "fn_0_basic_block_0");
            REQUIRE(V_names[way[1]] == "fn_0_basic_block_2");
            REQUIRE(V_names[way[2]] == "fn_0_basic_block_1");
        }
        else {
            REQUIRE(count_ans(g, initial) == 1);
            REQUIRE(g[initial][0][3] == 1);
            way = path_find(g_l, 0, 3, initial, last);
            REQUIRE(way.size() == 3);
            REQUIRE(V_names[way[0]] == "fn_1_basic_block_0");
            REQUIRE(V_names[way[1]] == "fn_1_basic_block_2");
            REQUIRE(V_names[way[2]] == "fn_1_basic_block_3");
        }
    }
}

TEST_CASE("llvm") {
    mkdir("data", 0777);
    std::string path = std::filesystem::current_path().string(), s1, s2;
    system(("python " + path + "/code/llvm.py " + "-llvm " + "-file " + path + "/input/llvm.in " + path + "/examples/.foo.dot " + path + "/examples/.main.dot").c_str());
    std::ifstream test_graph(path + "/tests/test_graph_llvm");
    std::ifstream graph(path + "/data/graph");
    REQUIRE(test_graph.is_open());
    REQUIRE(graph.is_open());
    for (int i = 0; i < 27; i++) {
        test_graph >> s1;
        graph >> s2;
        REQUIRE(s1 == s2);
    }
}