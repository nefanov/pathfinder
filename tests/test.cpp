#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "code2graph.h"
TEST_CASE("wrong path to the file") {
    std::ifstream input_file, analyze_file;
    REQUIRE(process_path(2, "./", input_file, analyze_file, 0) == -1);
}

TEST_CASE("using file input/test.in") {
    std::ifstream input_file, analyze_file;
    std::string path = "input/test.in", full_path = std::filesystem::current_path().string() + "/" + path;
    full_path.erase(full_path.find_last_of("/") + 1, full_path.size()); // .../gcc-cfg-utils/input/test.in -> .../gcc-cfg-utils/input/
    REQUIRE(process_path(3, path, input_file, analyze_file, 1) == 0);
    //REQUIRE(path == full_path + "../examples/test1.c" + ".012t.cfg.dot");
}

TEST_CASE("path to examples/test1.c") {
    std::ifstream input_file, analyze_file;
    REQUIRE(process_path(2, "examples/test1.c", input_file, analyze_file, 0) == 0);
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
			foo6(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, len);
		else
			foo5(inp, basic_block, code, Code, subgraph);
	}
    REQUIRE(Clusters[0].first == "cluster_foo");
    REQUIRE(Clusters[1].first == "cluster_main");
    REQUIRE(Clusters[0].second.first == 0);
    REQUIRE(Clusters[0].second.second == 1);
    REQUIRE(Clusters[1].second.first == 0);
    REQUIRE(Clusters[1].second.second == 1);
    REQUIRE(V[0][0] == "fn_0_basic_block_0");
    REQUIRE(Code[0][0] == "ENTRY\n");
    REQUIRE(V[0][1] == "fn_0_basic_block_1");
    REQUIRE(Code[0][1] == "EXIT\n");
    REQUIRE(V[0][2] == "fn_0_basic_block_2");
    //REQUIRE(Code[0][2] == "");
    REQUIRE(V[1][0] == "fn_1_basic_block_0");
    REQUIRE(Code[0][0] == "ENTRY\n");
    REQUIRE(V[1][1] == "fn_1_basic_block_1");
    REQUIRE(Code[0][1] == "EXIT\n");
    REQUIRE(V[1][2] == "fn_1_basic_block_2");
    //REQUIRE(Code[1][2] == "");
    REQUIRE(V[1][3] == "fn_1_basic_block_3");
    //REQUIRE(Code[1][3] == "");

    REQUIRE(V[0][E[0][0][0].first] == "fn_0_basic_block_2");
    REQUIRE(V[0][E[0][0][1].first] == "fn_0_basic_block_1");
    //REQUIRE(V[0][E[0][1][0].first] == "");
    REQUIRE(V[0][E[0][2][0].first] == "fn_0_basic_block_1");

    REQUIRE(V[1][E[1][0][0].first] == "fn_1_basic_block_2");
    REQUIRE(V[1][E[1][0][1].first] == "fn_1_basic_block_1");
    //REQUIRE(V[1][E[0][1][0].first] == "");
    REQUIRE(V[1][E[1][2][0].first] == "fn_1_basic_block_3");
    REQUIRE(V[1][E[1][3][0].first] == "fn_1_basic_block_1");

}