#include "code2graph.h"
#include "orchestrator.h"
enum {STDIN, CMD, FILEIN};
int process_path(class options& opt, std::ifstream& input_file, std::ifstream& analyze_file)
{
	std::string path_to_analyze = opt.path_to_analyzed_file;
	switch(opt.input_type) {
		case (STDIN):
			std::cout << "Enter the name of the file to be analyzed, pre-compiled with gcc file_name -fdump-tree-cfg-graph: ";
			std::cin >> path_to_analyze;
			path_to_analyze += opt.ir_file_suffix;
		break;
		case (CMD):
			path_to_analyze += opt.ir_file_suffix;
		break;
		case (FILEIN):
			input_file.open(opt.path_to_input_file);
			if (!input_file.is_open()) {
				std::cout << "file " << opt.path_to_input_file << " was not opened" << std::endl; 
				return -1;
			}
			input_file >> path_to_analyze;
			path_to_analyze += opt.ir_file_suffix;
		break;
	}
	analyze_file.open(path_to_analyze);
	if (!analyze_file.is_open()) {
			std::cout << "file " << path_to_analyze << " was not opened" << std::endl;
			return -1;
	}
	return 0;
}

void cluster_handler(int& len, size_t& found, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E)
{
	cluster++, subgraph++;
	int k;
	for(k = found + 18; k < len && inp[k] != '"'; k++);
	Clusters.push_back({inp.substr(found + 18, k - found - 18), {0,0}});
	V.resize(V.size() + 1);
	Code.resize(Code.size() + 1);
	E.resize(E.size() + 1);
}

void entry_end_handler(int& subgraph, int& k, std::string& inp, int len, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code)
{
	if (inp[len - 1] == '\\')
	{
		basic_block = 1;
		code = "";
	}
	else
	{
		for(k; k< len && inp[k] != '"'; k++);
		code = inp.substr(k + 1, len - k - 4) + "\n";
		Code[subgraph].push_back(code);
		code = "";
	}
}

void vertex_handler(std::string& code, int& basic_block, size_t& found1, int len, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph)
{
	int k = found1;
	while (k < len && inp[k] != ' ')
		k++;
	int k1 = found1;
	while (k1 >= 0 && inp[k1] != 'f')
		k1--;
	std::string a = inp.substr(k1, k - k1);
	if (a[a.size() - 1] == '0' && a[a.size() - 2] == '_')
		Clusters[subgraph].second.first = V[subgraph].size();
	else if (a[a.size() - 1] == '1' && a[a.size() - 2] == '_')
		Clusters[subgraph].second.second = V[subgraph].size();
	V[subgraph].push_back(make_pair(a, -1));
	E[subgraph].resize(E[subgraph].size() + 1);
	size_t found3 = inp.find("label=");
	k = found3;
	entry_end_handler(subgraph, k, inp, len, basic_block, code, Code);
}

void edges_handler(size_t& found1, size_t& found2, std::string& inp, std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph)
{
	int k1r = found1, k1l = 0, k2l = found2, k2r;
	while (inp[k1r] != ':')
		k1r++;
	std::string a = inp.substr(k1l + 1, k1r - k1l - 1);
	while (inp[k2l] != ' ')
		k2l++;
	k2r = k2l;
	while (inp[k2r] != ':')
		k2r++;
	std::string b = inp.substr(k2l + 1, k2r - k2l - 1);	
	int va, vb;
	for (int i = 0; i < V[subgraph].size(); i++)
	{
		if (V[subgraph][i].first == a)
			va = i;
		if (V[subgraph][i].first == b)
			vb = i;
	}
	E[subgraph][va].push_back({vb, ""});
}

void blocks_handler(std::string& code, int& basic_block, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int len)
{
	size_t found = inp.find("subgraph"), found1 = inp.find("basic_block"), found2 = inp.find("->");
	if (found != -1 && cluster == 0)
		cluster_handler(len, found, cluster, subgraph, inp, Clusters, V, Code, E);
	if (inp == "}")
		cluster = 0;
	if (found1 != -1 && found2 == -1)
		vertex_handler(code, basic_block, found1, len, inp, Clusters, V, Code, E, subgraph);
	else if (found1 != -1 && found2 != -1)
		edges_handler(found1, found2, inp, V, E, subgraph);
}

void code_handler(std::string& inp, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code, int& subgraph)
{
	if (inp == "}\"];")
	{
		basic_block = 0;
		Code[subgraph].push_back(code);
	}
	else
		code += inp + "\n";
}

void graph_list(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters)
{
	std::cout << "Clusters names and their start and finish vertexes" << std::endl;
	for (int i = 0; i < Clusters.size(); i++)
		std::cout << Clusters[i].first << " " << Clusters[i].second.first << " " << Clusters[i].second.second << std::endl;
	std::cout << std::endl;
}

void vertex_list(std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& Code)
{
	std::cout << "Vertexes' names and their code" << std::endl;
	for (int i = 0; i < V.size(); i++)
	{
		std::cout << Clusters[i].first << std::endl;
		for (int j = 0; j < V[i].size(); j++)
		{
			std::cout << V[i][j].first << std::endl;
			std::cout << Code[i][j] << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
	std::cout << std::endl;
}

void adjacency_list(std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E)
{
	std::cout << "Adjacency lists for each graph" << std::endl;
	for (int i = 0; i < E.size(); i++)
	{
		for (int j = 0; j < E[i].size(); j++)
		{
			std::cout << V[i][j].first << " : ";
			for (int q = 0; q < E[i][j].size(); q++)
				std::cout << V[i][E[i][j][q].first].first << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void new_graph_list(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::pair<std::string, int>>>& V)
{
	std::cout << "Clusters names and their start and finish vertexes" << std::endl;
	for (int i = 0; i < Clusters.size(); i++)
		std::cout << Clusters[i].first << " " << V[i][Clusters[i].second.first].second << " " << V[i][Clusters[i].second.second].second << std::endl;
	std::cout << std::endl;
}

void new_vertex_list(std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<int>>& V_new, std::vector<std::string>& Code_new)
{
	std::cout << "Vertexes' names and their code" << std::endl;
	for (int i = 0; i < V_new.size(); i++)
	{
		std::cout << i << " " << Clusters[V_new[i][0]].first << " " << V[V_new[i][0]][V_new[i][1]].first + "_" + std::to_string(V_new[i][2]) << std::endl;
		std::cout << Code_new[i] << std::endl;
	}
	std::cout << std::endl;
}

void new_adjacency_list(std::vector<std::vector<std::pair<int, std::string>>>& E_new)
{
	std::cout << "Adjacency list" << std::endl;
	for (int i = 0; i < E_new.size(); i++)
	{
		std::cout << i << " : ";
		for (int j = 0; j < E_new[i].size(); j++)
			std::cout << E_new[i][j].first << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void input_V_E(std::ifstream& fin, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::pair<std::string, std::string> >& rules, std::ifstream& input_file)
{
	std::cout << "Letters on edjes placing and grammar choosing processes " << std::endl;
	int quan_of_rules;
	std::cout << "Grammar" << std::endl;
	if (fin.is_open()) {
		fin >> quan_of_rules;
		std::cout << quan_of_rules << std::endl;
	}
	else
		std::cin >> quan_of_rules;
	std::string left, right;
	for (int j = 0; j < quan_of_rules; j++)
	{
		if (fin.is_open()) {
			fin >> left >> right;
			std::cout << left << " " << right << std::endl;
		}
		else
			std::cin >> left >> right;
		rules.push_back(make_pair(left, right));
	}
	for (int i = 0; i < E_new.size(); i++)
	{
		for (int j = 0; j < E_new[i].size(); j++)
		{
			std::cout << "Pick an letter for edge " << i << " -> " << E_new[i][j].first << " :";
			if (fin.is_open()) {
				fin >> E_new[i][j].second;
				std::cout << E_new[i][j].second << std::endl;
			}
			else
				std::cin >> E_new[i][j].second;
		}
	}
	input_file.close();
}

void visualising_graph(std::string& path_to_visualizator, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new,	std::vector<std::string>& Code_new, std::vector <std::vector<std::pair<std::string, int>>>& V)
{		
	std::streambuf *psbuf, *backup;
	std::ofstream fout;
	std::string dir_path = path_to_visualizator;
	std::cout << path_to_visualizator << " keklol\n"; 
	if (path_to_visualizator != "")
	{	
		int pointer = path_to_visualizator.find_last_of("/");
		if (pointer != std::string::npos) {	
			dir_path.erase(path_to_visualizator.find_last_of("/"), path_to_visualizator.size());
			system(("mkdir -p "+dir_path).c_str());
		}
		fout.open(path_to_visualizator);
		backup = std::cout.rdbuf(); 	
		std::cout.rdbuf(fout.rdbuf());
	}
	else
	{
		std::cout << "-----------------------------------------------\n";
		std::cout << "For watching how new graph looks like copy this to file.dot and use dot -Tpng file.dot -o filepic\n";
	}
	std::cout << "digraph code {\n";
	for (int i = 0; i < V_new.size(); i++) {
		std::cout << V[V_new[i][0]][V_new[i][1]].first + "_" + std::to_string(V_new[i][2]) << " [shape = record, label = \"" << Code_new[i] << "\"];\n";
 	}
	for (int i = 0; i < V_new.size(); i++) {
		for (int j = 0; j < E_new[i].size(); j++) {
			std::cout << V[V_new[i][0]][V_new[i][1]].first + "_" + std::to_string(V_new[i][2]) << " -> " << V[V_new[E_new[i][j].first][0]][V_new[E_new[i][j].first][1]].first + "_" + std::to_string(V_new[E_new[i][j].first][2]) << ";\n"; 
		}
	}
	std::cout << "}\n";
	if (path_to_visualizator != "")
	{
		std::cout.rdbuf(backup);
		fout.close();
	}	
	else
	{
		std::cout << "-----------------------------------------------\n";
	}
}

void to_fifo(std::string bin_path, std::vector<std::vector<int>>& V_new, std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::pair<std::string, std::string> >& rules)
{
	std::ofstream fout;
	mkdir("data", 0777);
	fout.open("data/graph");
	fout << 1 << std::endl;
	fout << rules.size() << std::endl;
	for (int i = 0; i < rules.size(); i++)
		fout << rules[i].first << " " << rules[i].second << std::endl;
	fout << std::endl;
	int Ee = 0;
	for (int i = 0; i < E_new.size(); i++)
		Ee += E_new[i].size();
	fout << V_new.size() << " " << Ee << std::endl;
	for (int i = 0; i < V_new.size(); i++)
		fout << V[V_new[i][0]][V_new[i][1]].first + "_" + std::to_string(V_new[i][2]) << " ";
	fout << std::endl;
	for (int i = 0; i < E_new.size(); i++)
		for (int j = 0; j < E_new[i].size(); j++)
			fout << i << " " << E_new[i][j].first << " " << E_new[i][j].second << std::endl;
	fout << std::endl;
	fout.close();
}

int number_of_file_arg(int argc, char* argv[], const char* arg)
{
	for (int i = 0; i < argc; i++)
		if(strcmp(argv[i], arg) == 0)
			return i;
	return -1;
}

int get_input_type(int argc, char* argv[])
{
	if (number_of_file_arg(argc, argv, "-file") > 0)
		return 2;
	if (number_of_file_arg(argc, argv, "-cmd") > 0)
		return 1;
	return 0;
}
