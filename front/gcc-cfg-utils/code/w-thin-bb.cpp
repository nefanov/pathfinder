#include "code2graph.h"

extern "C" {
void new_graph_creator(std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)
{
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V[i].size(); j++) {
			V[i][j].second = V_new.size();
			std::string code = Code[i][j];
			int found = -1, counter = 0;
			while (found != code.size() - 1) {
				code = code.substr(found + 1, code.size() - found - 1);
				if (code.substr(0, 4) == "|if\\")
					Code_new.push_back(code), found = code.size() - 1;
				else
					found = code.find('\n'), Code_new.push_back(code.substr(0, found + 1));
				V_new.push_back({i, j, counter});
				E_new.resize(V_new.size());
				if (counter != 0)
					E_new[E_new.size() - 2].push_back(std::make_pair(E_new.size() - 1, ""));
				counter++;
			}
		}
	}
	for (int i = 0; i < E.size(); i++) {
		for (int j = 0; j < E[i].size(); j++) {
			for (int q = 0; q < E[i][j].size(); q++) {
				int s = V[i][j].second, f = V[i][E[i][j][q].first].second;
				for (s; s + 1 < V_new.size() && V_new[s + 1][2] != 0; s++);
				E_new[s].push_back(std::make_pair(f, ""));
			}
		}
	}
}
void graph_merger(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)
{
	for (int i = 0; i < V_new.size(); i++) {
		for (int j = 0; j < Clusters.size(); j++) {
			std::string func_name_1 = "|" + Clusters[j].first + "\\";
			std::string func_name_2 = "\\ " + Clusters[j].first + "\\";
			int found_1 = Code_new[i].find(func_name_1);
			int found_2 = Code_new[i].find(func_name_2);
			if (found_1 != std::string::npos || found_2 != std::string::npos) {
				E_new[V[j][Clusters[j].second.second].second].insert(E_new[V[j][Clusters[j].second.second].second].end(), E_new[i].begin(), E_new[i].end());
				E_new[i].clear();
				E_new[i].push_back(std::make_pair(V[j][Clusters[j].second.first].second, ""));
			}
		}
	}
}
}