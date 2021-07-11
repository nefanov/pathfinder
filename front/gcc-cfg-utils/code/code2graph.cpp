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
	std::cout << "-----------------------------------------------------------------\nNew Graph\n\n";
	std::vector<std::vector<int>> V_new; // V_new - (graph number, vertex number in graph, number of new_vertex in old_vertex)
	std::vector<std::string> Code_new;
	std::vector<std::vector<std::pair<int, std::string>>> E_new; 
	new_graph_creator(V, E, Code, V_new, E_new, Code_new);
	graph_merger(Clusters, V, E, Code, V_new, E_new, Code_new);
	new_graph_list(Clusters, V);
	new_vertex_list(V, Clusters, V_new, Code_new);
	new_adjacency_list(E_new);
	std::vector<std::pair<std::string, std::string> > rules;
	input_V_E(input_file, file, E_new, rules, analyze_file);
	to_fifo(bin_path, V_new, V, E_new, rules);
  	execl((bin_path + "core").c_str(), (bin_path + "core").c_str(), (bin_path).c_str(), NULL);
	return -1;
}