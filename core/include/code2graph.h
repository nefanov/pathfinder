#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <filesystem>

int process_path       (int argc, std::ifstream& input_file, std::string& path, 
                        std::string& path_to_input, std::ifstream& analyze_file, int file);

void to_fifo           (std::string bin_path, std::vector <std::vector<std::string>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, 
                        std::vector<std::vector<std::pair<std::string, std::string> > >& rules);

void input_V_E         (std::ifstream& fin, int file, std::vector <std::vector<std::string>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector<std::vector<std::pair<std::string, std::string> > >& rules, 
                        std::ifstream& input_file);

void adjacency_list    (std::vector <std::vector<std::string>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);

void vertex_list       (std::vector <std::vector<std::string>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::string>>& Code);

void graph_list        (std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters);

void cluster_handler   (int& len, size_t& found, int& cluster, int& subgraph, std::string& inp, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);

void entry_end_handler (int& subgraph, int& k, std::string& inp, int& len, int& basic_block, 
                        std::string& code, std::vector <std::vector<std::string>>& Code);

void vertex_handler    (std::string& code, int& basic_block, size_t& found1, int& len, std::string& inp, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);

void edges_handler     (int& found1, size_t& found2, std::string& inp, std::vector <std::vector<std::string>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);

void code_handler      (std::string& inp, int& basic_block, std::string& code, 
                        std::vector <std::vector<std::string>>& Code, int& subgraph);

void blocks_handler    (std::string& code, int& basic_block, int& cluster, int& subgraph, 
                        std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::string>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& len);