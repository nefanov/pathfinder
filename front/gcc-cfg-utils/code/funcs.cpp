#include "code2graph.h"

int process_path(int argc, std::ifstream& input_file, std::string& path, std::string& path_to_input, std::ifstream& analyze_file, int file)
{
	std::string path_to_analyze = path, full_path = std::filesystem::current_path().string() + "/" + path;
    if (argc < 2) {
		std::cout << "Enter the name of the file to be analyzed, pre-compiled with gcc file_name -fdump-tree-cfg-graph: ";
		std::cin >> path_to_analyze;
	}
	if (file) {
		input_file.open(full_path);
		path_to_input = full_path;
		input_file >> path_to_analyze;
		full_path.erase(full_path.find_last_of("/") + 1, full_path.size()); // .../gcc-cfg-utils/input/test.in -> .../gcc-cfg-utils/input/
		path_to_analyze = full_path + path_to_analyze + ".012t.cfg.dot";
		//input_file.close();
	}
	else
		path_to_analyze = std::filesystem::current_path().string() + "/" + path_to_analyze + ".012t.cfg.dot";
	analyze_file.open(path_to_analyze);
	path = path_to_analyze;
	if (!analyze_file.is_open()) {
		std::cout << "file " << path_to_analyze << " was not opened" << std::endl;
		return -1;
	}
    return 0;
}

void to_fifo(std::string bin_path, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::pair<std::string, std::string> > >& rules)
{
	std::ofstream fout;
	mkdir((bin_path + "../data").c_str(), 0777);
	fout.open(bin_path + "../data/graph");
	fout << E.size() << std::endl;
	for (int i = 0; i < V.size(); i++)
	{
		fout << rules[i].size() << std::endl;
		for (int j = 0; j < rules[i].size(); j++)
			fout << rules[i][j].first << " " << rules[i][j].second << std::endl;
		fout << std::endl;
		int Vv = 0, Ee = 0;
		Vv = V[i].size();
		for (int j = 0; j < V[i].size(); j++)
			Ee += E[i][j].size();
		fout << Vv << " " << Ee << std::endl;
		for (int j = 0; j < Vv; j++)
			fout << V[i][j] << " ";
		fout << std::endl;
		for (int j = 0; j < V[i].size(); j++)
			for (int q = 0; q < E[i][j].size(); q++)
				fout << j << " " << E[i][j][q].first << " " << E[i][j][q].second << std::endl;
		fout << std::endl;
	}
	fout.close();
}

void input_V_E(std::ifstream& fin, int file, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, std::string> > >& rules, std::ifstream& input_file)
{
	std::cout << "Процесс установки букв на ребра и выбора грамматик для графов " << std::endl;
	for (int i = 0, quan_of_rules; i < E.size(); i++)
	{
		std::cout << "Грамматика и граф для " << Clusters[i].first << std::endl;
		if (file == 1) {
			fin >> quan_of_rules;
			std::cout << quan_of_rules << std::endl;
		}
		else
			std::cin >> quan_of_rules;
		std::string left, right;
		for (int j = 0; j < quan_of_rules; j++)
		{
			if (file == 1) {
				fin >> left >> right;
				std::cout << left << " " << right << std::endl;
			}
			else
				std::cin >> left >> right;
			rules[i].push_back(make_pair(left, right));
		}
		for (int j = 0; j < E[i].size(); j++)
		{
			for (int q = 0; q < E[i][j].size(); q++)
			{
				std::cout << "Укажите букву для ребра " << V[i][j] << " -> " << V[i][E[i][j][q].first] << " :";
				if (file == 1) {
					fin >> E[i][j][q].second;
					std::cout <<E[i][j][q].second << std::endl;
				}
				else
					std::cin >> E[i][j][q].second;
			}
		}
	}
	input_file.close();
}

void adjacency_list(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E)
{
	std::cout << "Списки смежности для каждого графа" << std::endl;
	for (int i = 0; i < E.size(); i++)
	{
		for (int j = 0; j < E[i].size(); j++)
		{
			std::cout << V[i][j] << " : ";
			for (int q = 0; q < E[i][j].size(); q++)
				std::cout << V[i][E[i][j][q].first] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void vertex_list(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& Code)
{
	std::cout << "Список названий вершин каждого графа и их код" << std::endl;
	for (int i = 0; i < V.size(); i++)
	{
		std::cout << Clusters[i].first << std::endl;
		for (int j = 0; j < V[i].size(); j++)
		{
			std::cout << V[i][j] << std::endl;
			std::cout << Code[i][j] << std::endl;
		}
		std::cout << std::endl << std::endl;
	}
	std::cout << std::endl;
}

void graph_list(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters)
{
	std::cout << "Названия кластеров под различные графы, начальная и конечная вершины для них" << std::endl;
	for (int i = 0; i < Clusters.size(); i++)
		std::cout << Clusters[i].first << " " << Clusters[i].second.first << " " << Clusters[i].second.second << std::endl;
	std::cout << std::endl;
}

void cluster_handler(int& len, size_t& found, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E)
{
	cluster++, subgraph++;
	int k;
	for(k = found + 10; k < len && inp[k] != '"'; k++);
	Clusters.push_back({inp.substr(found + 10, k - found - 10), {0,0}});
	V.resize(V.size() + 1);
	Code.resize(Code.size() + 1);
	E.resize(E.size() + 1);
}


void entry_end_handler(int& subgraph, int& k, std::string& inp, int& len, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code)
{
	if (inp[len - 1] == '\\')
	{
		basic_block = 1;
		for(k; k< len && inp[k] != '"'; k++);
		code = inp.substr(k + 1, len - k) + "\n";
	}
	else
	{
		for(k; k< len && inp[k] != '"'; k++);
		code = inp.substr(k + 1, len - k - 4) + "\n";
		Code[subgraph].push_back(code);
		code = "";
	}
}

void vertex_handler(std::string& code, int& basic_block, size_t& found1, int& len, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph)
{
	int k = found1;
	while (k < len && inp[k] != ' ')
		k++;
	std::string a = inp.substr(found1 - 5, k - found1 + 5);
	if (a[a.size() - 1] == '0' && a[a.size() - 2] == '_')
		Clusters[subgraph].second.first = V[subgraph].size();
	else if (a[a.size() - 1] == '1' && a[a.size() - 2] == '_')
		Clusters[subgraph].second.second = V[subgraph].size();
	V[subgraph].push_back(a);
	E[subgraph].resize(E[subgraph].size() + 1);
	size_t found3 = inp.find("label=");
	k = found3;
	entry_end_handler(subgraph, k, inp, len, basic_block, code, Code);
}

void edges_handler(size_t& found1, size_t& found2, std::string& inp, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph)
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
		if (V[subgraph][i] == a)
			va = i;
		if (V[subgraph][i] == b)
			vb = i;
	}
	E[subgraph][va].push_back({vb, ""});
}

void code_handler(std::string& inp, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code, int& subgraph)
{
	if (inp == "}\"];")
	{
		basic_block = 0;
		code += "}\n";
		Code[subgraph].push_back(code);
	}
	else
		code += inp + "\n";
}

void blocks_handler(std::string& code, int& basic_block, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& len)
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