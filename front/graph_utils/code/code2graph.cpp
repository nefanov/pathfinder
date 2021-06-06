#include "header.h"

int main()
{
	std::string file, inp, graphfile, code = "";
	std::cout << "Введите название файла для анализа, предварительно скомпилируйте его командой  gcc file_name -fdump-tree-cfg-graph: ";
	std::cin >> file;
	graphfile = file;
	graphfile += ".012t.cfg.dot";
	std::ifstream input_file(graphfile);
	int code_descr = 0, cluster = 0, edgeline = 0, basic_block = 0, subgraph = -1;
	std::vector <std::pair<std::string, std::pair<int, int>>> Clusters;
	std::vector <std::vector<std::string>> V, Code;
	std::vector <std::vector<std::vector<std::pair<int, std::string>>>> E;
	while(std::getline(input_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0)
		{
			size_t found = inp.find("subgraph");
			if (found != -1 && cluster == 0)
			{
				cluster++, subgraph++;
				int k = found + 10;
				while (k < len && inp[k] != '"')
					k++;
				std::string a = inp.substr(found + 10, k - found - 10);
				Clusters.push_back(std::make_pair(a, std::make_pair(0, 0)));
				V.resize(V.size() + 1);
				Code.resize(Code.size() + 1);
				E.resize(E.size() + 1);
			}
			if (inp == "}")
				cluster = 0;
			size_t found1 = inp.find("basic_block");
			size_t found2 = inp.find("->");
			if (found1 != -1 && found2 == -1)
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
				if (inp[len - 1] == 92)
				{
					basic_block = 1;
					size_t found3 = inp.find("label=");
					k = found3;
					while (k < len && inp[k] != '"')
						k++;
					code = inp.substr(k + 1, len - k);
					code += '\n';
				}
				else
				{
					size_t found3 = inp.find("label=");
					k = found3;
					while (k < len && inp[k] != '"')
						k++;
					code = inp.substr(k + 1, len - k - 4);
					code += '\n';
					Code[subgraph].push_back(code);
					code = "";
				}

			}
			else if (found1 != -1 && found2 != -1)
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
				E[subgraph][va].push_back(std::make_pair(vb, ""));
			}
		}
		else
		{
			std::string a = "}1];";
			a[1] = '"';
			if (inp == a)
			{
				basic_block = 0;
				code += "}\n";
				Code[subgraph].push_back(code);
			}
			else
			{
				code += inp;
				code += '\n';
			}
		}
	}
	std::cout << "Названия кластеров под различные графы, начальная и конечная вершины для них" << std::endl;
	for (int i = 0; i < Clusters.size(); i++)
		std::cout << Clusters[i].first << " " << Clusters[i].second.first << " " << Clusters[i].second.second << std::endl;
	std::cout << std::endl;
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
	std::vector<std::vector<std::pair<std::string, std::string> > > rules(E.size());
	std::cout << "Процесс установки букв на ребра и выбора грамматик для графов " << std::endl;
	for (int i = 0, quan_of_rules; i < E.size(); i++)
	{
		std::cout << "Грамматика и граф для " << Clusters[i].first << std::endl;
		std::cin >> quan_of_rules;
		std::string left, right;
		for (int j = 0; j < quan_of_rules; j++)
		{
			std::cin >> left >> right;
			rules[i].push_back(make_pair(left, right));
		}
		for (int j = 0; j < E[i].size(); j++)
		{
			for (int q = 0; q < E[i][j].size(); q++)
			{
				std::cout << "Укажите букву для ребра " << V[i][j] << " -> " << V[i][E[i][j][q].first] << " :";
				std::cin >> E[i][j][q].second;
			}
		}
	}
	input_file.close();
	std::ofstream fout;
	mkdir("../front/graph_utils/data", 0777);
	fout.open("../front/graph_utils/data/graph");
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
  	execl("core", " ", NULL);
}
