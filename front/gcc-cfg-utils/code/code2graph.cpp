#include "code2graph.h"

int main(int argc, char* argv[])
{
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::string>> V, Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	std::string inp, code = "", path = (argc >= 2) ? argv[1] : "";
	std::ifstream input_file;

	process_path(argc, path, input_file);
	while(std::getline(input_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0)
			foo6(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, len);
		else
			foo5(inp, basic_block, code, Code, subgraph);
	}
	graph_list(Clusters);
	vertex_list(V, E, Clusters, Code);
	adjacency_list(V, E);
	std::vector<std::vector<std::pair<std::string, std::string> > > rules(E.size());
	input_V_E(V, E, Clusters, rules, input_file);
	to_fifo(V, E, rules);
  	execl("core", " ", NULL);
	return -1;
}