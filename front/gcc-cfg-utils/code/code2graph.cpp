#include "code2graph.h"

int main(int argc, char* argv[])
{
	std::string inp, code = "", path = (argc >= 2) ? argv[1] : "";
	std::ifstream input_file;
	process_path(argc, path, input_file);
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::string>> V, Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	while(std::getline(input_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0)
			foo6(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, len);
		else
			foo5(inp, basic_block, code, Code, subgraph);
		//std::cout << "19\n";
	}
	std::cout << "21\n";
	graph_list(Clusters);
	std::cout << "23\n";
	vertex_list(V, E, Clusters, Code);
	std::cout << "25\n";
	adjacency_list(V, E);
	std::cout << "27\n";
	std::vector<std::vector<std::pair<std::string, std::string> > > rules(E.size());
	input_V_E(V, E, Clusters, rules, input_file);
	to_fifo(V, E, rules);
  	execl("core", " ", NULL);
}
