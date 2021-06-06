#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

int main()
{
	string file;
	cout << "Введите название файла для анализа, предварительно скомпилируйте его командой  gcc file_name -fdump-tree-cfg-graph: ";
	cin >> file;
	string graphfile = file;
	graphfile += ".012t.cfg.dot";
	ifstream input_file(graphfile);
	string inp;
	int code_descr = 0;
	int cluster = 0;
	int edgeline = 0;
	int basic_block = 0;
	int subgraph = -1;
	vector <pair<string, pair<int, int>>> Clusters;
	vector <vector<string>> V;
	vector <vector<string>> Code;
	vector <vector<vector<pair<int, string>>>> E;
	string code = "";
	while(getline(input_file, inp)) 
	{
		int len = inp.size();
		if (basic_block == 0)
		{
			size_t found = inp.find("subgraph");
			if (found != -1 && cluster == 0)
			{
				cluster++;
				subgraph++;
				int k = found + 10;
				while (k < len && inp[k] != '"')
				{
					k++;
				}
				string a = inp.substr(found + 10, k - found - 10);
				Clusters.push_back(make_pair(a, make_pair(0, 0)));
				V.resize(V.size() + 1);
				Code.resize(Code.size() + 1);
				E.resize(E.size() + 1);
			}
			if (inp == "}")
			{
				cluster = 0;
			}
			size_t found1 = inp.find("basic_block");
			size_t found2 = inp.find("->");
			if (found1 != -1 && found2 == -1)
			{
				int k = found1;
				while (k < len && inp[k] != ' ')
				{
					k++;
				}
				string a = inp.substr(found1 - 5, k - found1 + 5);
				if (a[a.size() - 1] == '0' && a[a.size() - 2] == '_')
				{
					Clusters[subgraph].second.first = V[subgraph].size();
				}
				else if (a[a.size() - 1] == '1' && a[a.size() - 2] == '_')
				{
					Clusters[subgraph].second.second = V[subgraph].size();
				}
				V[subgraph].push_back(a);
				E[subgraph].resize(E[subgraph].size() + 1);
				if (inp[len - 1] == 92)
				{
					basic_block = 1;
					size_t found3 = inp.find("label=");
					k = found3;
					while (k < len && inp[k] != '"')
					{
						k++;
					}
					code = inp.substr(k + 1, len - k);
					code += '\n';
				}
				else
				{
					size_t found3 = inp.find("label=");
					k = found3;
					while (k < len && inp[k] != '"')
					{
						k++;
					}
					code = inp.substr(k + 1, len - k - 4);
					code += '\n';
					Code[subgraph].push_back(code);
					code = "";
				}

			}
			else if (found1 != -1 && found2 != -1)
			{
				int k1r = found1;
				while (inp[k1r] != ':')
				{
					k1r++;
				}
				int k1l = 0;
				string a = inp.substr(k1l + 1, k1r - k1l - 1);
				int k2l = found2;
				while (inp[k2l] != ' ')
				{
					k2l++;
				}
				int k2r = k2l;
				while (inp[k2r] != ':')
				{
					k2r++;
				}
				string b = inp.substr(k2l + 1, k2r - k2l - 1);
				int va, vb;
				for (int i = 0; i < V[subgraph].size(); i++)
				{
					if (V[subgraph][i] == a)
					{
						va = i;
					}
					if (V[subgraph][i] == b)
					{
						vb = i;
					}
				}
				E[subgraph][va].push_back(make_pair(vb, ""));
			}
		}
		else
		{
			string a = "}1];";
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
	cout << "Названия кластеров под различные графы, начальная и конечная вершины для них" << endl;
	for (int i = 0; i < Clusters.size(); i++)
	{
		cout << Clusters[i].first << " " << Clusters[i].second.first << " " << Clusters[i].second.second << endl;
	}
	cout << endl;
	cout << "Список названий вершин каждого графа и их код" << endl;
	for (int i = 0; i < V.size(); i++)
	{
		cout << Clusters[i].first << endl;
		for (int j = 0; j < V[i].size(); j++)
		{
			cout << V[i][j] << endl;
			cout << Code[i][j] << endl;
		}
		cout << endl << endl;
	}
	cout << endl;
	cout << "Списки смежности для каждого графа" << endl;
	for (int i = 0; i < E.size(); i++)
	{
		for (int j = 0; j < E[i].size(); j++)
		{
			cout << V[i][j] << " : ";
			for (int q = 0; q < E[i][j].size(); q++)
			{
				cout << V[i][E[i][j][q].first] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl;
	vector<vector<pair<string, string> > > rules(E.size());
	cout << "Процесс установки букв на ребра и выбора грамматик для графов " << endl;
	for (int i = 0; i < E.size(); i++)
	{
		cout << "Грамматика и граф для " << Clusters[i].first << endl;
		int quan_of_rules;
		cin >> quan_of_rules;
		string left, right;
		for (int j = 0; j < quan_of_rules; j++)
		{
			cin >> left >> right;
			rules[i].push_back(make_pair(left, right));
		}
		for (int j = 0; j < E[i].size(); j++)
		{
			for (int q = 0; q < E[i][j].size(); q++)
			{
				cout << "Укажите букву для ребра " << V[i][j] << " -> " << V[i][E[i][j][q].first] << " :";
				cin >> E[i][j][q].second;
			}
		}
	}
	input_file.close();
	ofstream fout;
	mkdir("../data", 0777);
	fout.open("../data/graph");
	fout << E.size() << endl;
	for (int i = 0; i < V.size(); i++)
	{
		fout << rules[i].size() << endl;
		for (int j = 0; j < rules[i].size(); j++)
		{
			fout << rules[i][j].first << " " << rules[i][j].second << endl;
		}
		fout << endl;
		int Vv = 0, Ee = 0;
		Vv = V[i].size();
		for (int j = 0; j < V[i].size(); j++)
		{
			Ee += E[i][j].size();
		}
		fout << Vv << " " << Ee << endl;
		for (int j = 0; j < Vv; j++)
		{
			fout << V[i][j] << " ";
		}
		fout << endl;
		for (int j = 0; j < V[i].size(); j++)
		{
			for (int q = 0; q < E[i][j].size(); q++)
			{
				fout << j << " " << E[i][j][q].first << " " << E[i][j][q].second << endl;
			}
		}
		fout << endl;
	}
	fout.close();
  	execl("../../../core/src/CYKonGraph", " ", NULL);
}
