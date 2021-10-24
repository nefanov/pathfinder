#include "code2graph.h"

extern "C" {
void new_graph_creator(std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)
{
	for (int i = 0; i < V.size(); i++)
		for (int j = 0; j < V[i].size(); j++)
			V[i][j].second = V_new.size(), V_new.push_back({i, j, 0}), Code_new.push_back(Code[i][j]);
	E_new.resize(V_new.size());
	for (int i = 0; i < E.size(); i++)
		for (int j = 0; j < E[i].size(); j++)
			for (int q = 0; q < E[i][j].size(); q++)
				E_new[V[i][j].second].push_back(std::make_pair(V[i][E[i][j][q].first].second, E[i][j][q].second));
}

void graph_merger(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)
{
	for (int i = 0; i < V_new.size(); i++)
	{
		int foundmin = Code_new[i].size(), cl_min = -1;
		for (int j = 0; j < Clusters.size(); j++)
		{
			int FLAG = 0, found = 0, found1 = 0;
			while (1)
			{
				std::string func_name_1 = "|" + Clusters[j].first + "\\";
				std::string func_name_2 = "\\ " + Clusters[j].first + "\\";
				int found_1 = Code_new[i].find(func_name_1);
				int found_2 = Code_new[i].find(func_name_2);
				if (found_1 == std::string::npos)
				{
					found = found_2;
				}
				else
				{
					found = found_1;
				}
				if (found != std::string::npos)
				{
					int counter = 0;
					for (int q = 0; q < found; q++)
						if (Code_new[i][q] == '"')
							counter++;
					if (counter % 2 == 0)
					{
						FLAG = 1;
						break;
					}
					found1 = Code_new[i].find('\n', found);
				}
				else
					break;
			}
			if (FLAG == 1 && found < foundmin)
				foundmin = found, cl_min = j;
		}		
		if (cl_min != -1)
		{
			int found1 = Code_new[i].find('\n', foundmin);
			if (found1 != Code_new[i].size() - 1) {
				std::string code = Code_new[i].substr(found1 + 1, Code_new[i].size() - found1 - 1);
				Code_new[i] = Code_new[i].substr(0, found1 + 1);
				V_new.push_back({V_new[i][0], V_new[i][1], V_new[i][2] + 1});
				Code_new.push_back(code);
				E_new.push_back(E_new[i]);
				E_new[V[cl_min][Clusters[cl_min].second.second].second].push_back(std::make_pair(E_new.size() - 1, ""));
			}
			else 
				E_new[V[cl_min][Clusters[cl_min].second.second].second].insert(E_new[V[cl_min][Clusters[cl_min].second.second].second].end(), E_new[i].begin(), E_new[i].end());
			E_new[i].clear();
			E_new[i].push_back(std::make_pair(V[cl_min][Clusters[cl_min].second.first].second, ""));
		}
	}
}
}