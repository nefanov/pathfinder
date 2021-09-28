#include "code2graph.h"
#include "orchestrator.h"
int main(int argc, char* argv[])
{
	options opt(argc, argv);
	char* bpath = (char*)malloc(PATH_MAX);
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::pair<std::string, int>>> V; //V-<block_name, number in V_new>
	std::vector <std::vector<std::string>> Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	std::string inp, code = "", bin_path = realpath(argv[0], bpath);
	std::ifstream analyze_file, input_file;
	bin_path.erase(bin_path.find_last_of("/") + 1, bin_path.size()); // .../gcc-cfg-utils/build/code2graph -> .../gcc-cfg-utils/build/
	if (process_path(opt, input_file, analyze_file) < 0)
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
	void* sl = dlopen((opt.is_thin ? "libthin.so" : "libfat.so"), RTLD_LAZY);
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
	int inp_check = input_file.peek();
        if (inp_check != EOF) {
                std::vector<std::pair<std::string, std::string> > rules;
                input_V_E(input_file, E_new, rules, analyze_file);
                visualising_graph(opt.path_to_save_file, V_new, E_new, Code_new, V);
                to_fifo(bin_path, V_new, V, E_new, rules);
                if (!opt.is_front_only)
                        execl((bin_path + "core").c_str(), (bin_path + "core").c_str(), "data/graph", (opt.is_fast) ? "-fast" : "-slow", NULL);
        } else {
                visualising_graph(opt.path_to_save_file, V_new, E_new, Code_new, V);
        }
	return 0;
}
