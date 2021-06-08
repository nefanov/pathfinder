#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

int process_path(int argc, std::string path, std::ifstream& input_file);
void to_fifo(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::pair<std::string, std::string> > >& rules);
void input_V_E(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, std::string> > >& rules, std::ifstream& input_file);
void adjacency_list(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);
void vertex_list(std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& Code);
void graph_list(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters);
void foo1(int& len, size_t& found, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);
void foo2(int& subgraph, int& k, std::string& inp, int& len, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code);
void foo3(std::string& code, int& basic_block, size_t& found1, int& len, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);
void foo4(int& found1, size_t& found2, std::string& inp, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);
void foo5(std::string& inp, int& basic_block, std::string& code, std::vector <std::vector<std::string>>& Code, int& subgraph);
void foo6(std::string& code, int& basic_block, int& cluster, int& subgraph, std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& len);