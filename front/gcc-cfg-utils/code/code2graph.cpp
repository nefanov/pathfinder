#include "code2graph.h"

int main(int argc, char* argv[])
{
	char* bpath = (char*)malloc(PATH_MAX);
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1, file_arg_number = number_of_file_arg(argc, argv, "-file") + 1, cmd_arg_number = number_of_file_arg(argc, argv, "-cmd") + 1, input_type = get_input_type(argc, argv);
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::pair<std::string, int>>> V; //V-<block_name, number in V_new>
	std::vector <std::vector<std::string>> Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	std::string path_to_visualizator = "", path = (cmd_arg_number > 0) ? argv[cmd_arg_number] : "", inp, code = "", bin_path = realpath(argv[0], bpath), path_to_input = (file_arg_number > 0) ? argv[file_arg_number] : "";
	std::ifstream analyze_file, input_file;
	bin_path.erase(bin_path.find_last_of("/") + 1, bin_path.size()); // .../gcc-cfg-utils/build/code2graph -> .../gcc-cfg-utils/build/
	if (process_path(input_type, argc, input_file, path, path_to_input, analyze_file) < 0)
		return -1;
	while(std::getline(analyze_file, inp)) 
		(basic_block == 0) ? blocks_handler(code, basic_block, cluster, subgraph, inp, Clusters, V, Code, E, inp.size()): code_handler(inp, basic_block, code, Code, subgraph);
	graph_list(Clusters);
	vertex_list(V, E, Clusters, Code);
	adjacency_list(V, E);
	std::cout << "-----------------------------------------------------------------\nNew Graph\n\n";
	std::vector<std::vector<int>> V_new; // V_new - (graph number, vertex number in graph, number of new_vertex in old_vertex)
	std::vector<std::string> Code_new;
	std::vector<std::vector<std::pair<int, std::string>>> E_new;
	int onumarg = number_of_file_arg(argc, argv, "-o") + 1;
	if (onumarg > 0)
		path_to_visualizator = argv[onumarg];
	bool thin = (number_of_file_arg(argc, argv, "-thin") > 0) ? true : false;
	void* sl = dlopen(((thin == true) ? "libthin.so" : "libfat.so"), RTLD_LAZY);
	if (sl == NULL) {
		fprintf(stderr, "%s\n", dlerror());
		return -1;
	}  
	void* ngc = dlsym(sl, "new_graph_creator");
	if (ngc == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }
	void* gm = dlsym(sl, "graph_merger");
	if (gm == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }
	funcs_front func(ngc, gm);
	func.new_graph_creator(V, E, Code, V_new, E_new, Code_new);
	func.graph_merger(Clusters, V, E, Code, V_new, E_new, Code_new);
	new_graph_list(Clusters, V);
	new_vertex_list(V, Clusters, V_new, Code_new);
	new_adjacency_list(E_new);
	std::vector<std::pair<std::string, std::string> > rules;
	input_V_E(input_file, E_new, rules, analyze_file);
	visualising_graph(path_to_visualizator, V_new, E_new, Code_new, V);
	to_fifo(bin_path, V_new, V, E_new, rules);
	if (number_of_file_arg(argc, argv, "-front-only") <= 0)
  		execl((bin_path + "core").c_str(), (bin_path + "core").c_str(), "data/graph", NULL);
	return 0;
}