#include "code2graph.h"

int main(int argc, char* argv[])
{
	char* bpath = (char*)malloc(PATH_MAX);
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1, path_arg_number = number_of_file_arg(argc, argv, "-file") + 1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::pair<std::string, int>>> V; //V-<block_name, number in V_new>
	std::vector <std::vector<std::string>> Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	std::string path = (path_arg_number > 0) ? argv[1] : "", inp, code = "", bin_path = realpath(argv[0], bpath), path_to_input = (path_arg_number > 0) ? argv[path_arg_number] : "";
	std::ifstream analyze_file, input_file;
	bin_path.erase(bin_path.find_last_of("/") + 1, bin_path.size()); // .../gcc-cfg-utils/build/code2graph -> .../gcc-cfg-utils/build/
	process_path(argc, input_file, path, path_to_input, analyze_file);
	while(std::getline(analyze_file, inp)) 
		(basic_block == 0) ? blocks_handler(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, inp.size()): code_handler(inp, basic_block, code, Code, subgraph);
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
	input_V_E(input_file, E_new, rules, analyze_file);
	visualising_graph(V_new, E_new, Code_new, V);
	to_fifo(bin_path, V_new, V, E_new, rules);
	if (number_of_file_arg(argc, argv, "-front-only") <= 0)
  		execl((bin_path + "core").c_str(), (bin_path + "core").c_str(), (bin_path).c_str(), NULL);
	return -1;
}