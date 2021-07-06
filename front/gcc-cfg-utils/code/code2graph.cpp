#include "code2graph.h"

int main(int argc, char* argv[])
{
	char* bpath = (char*)malloc(PATH_MAX);
	std::string arg = (argc >= 2) ? argv[1] : "";
	int file = (argc >= 2) ? ((arg == "-file") ? 1 : 0) : 0, code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::pair<std::string, int>>> V; //V-<block_name, number in V_new>
	std::vector <std::vector<std::string>> Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	std::string path_to_input = "", inp, code = "", bin_path = realpath(argv[0], bpath), path = (arg == "-file") ? argv[2] : arg;
	std::ifstream analyze_file, input_file;
	
	bin_path.erase(bin_path.find_last_of("/") + 1, bin_path.size()); // .../gcc-cfg-utils/build/code2graph -> .../gcc-cfg-utils/build/
	process_path(argc, input_file, path, path_to_input, analyze_file, file);
	while(std::getline(analyze_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0) // input is not a code
			blocks_handler(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, len);
		else
			code_handler(inp, basic_block, code, Code, subgraph);
	}
	graph_list(Clusters);
	vertex_list(V, E, Clusters, Code);
	adjacency_list(V, E);
	std::vector<std::vector<std::pair<std::string, std::string> > > rules(E.size());
	input_V_E(input_file, file, V, E, Clusters, rules, analyze_file);
	to_fifo(bin_path, V, E, rules);
  	execl((bin_path + "core").c_str(), (bin_path + "core").c_str(), (bin_path).c_str(), NULL);
	return -1;
}