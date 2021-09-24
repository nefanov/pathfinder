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
#include <dlfcn.h>

class funcs_front
{
    public:
    void (*new_graph_creator)(std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new);
    void (*graph_merger)(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new);

    funcs_front(void* a, void* b)
    {
        new_graph_creator = reinterpret_cast<void(*)(std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)>(a);
        graph_merger = reinterpret_cast<void(*)(std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, std::vector<std::vector<std::pair<std::string, int>>>& V, std::vector<std::vector<std::vector<std::pair<int, std::string>>>>& E, std::vector<std::vector<std::string>>& Code, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new, std::vector<std::string>& Code_new)>(b);
    }

};

int process_path       (class options& opt, std::ifstream& input_file, std::ifstream& analyze_file);

void cluster_handler   (int& len, size_t& found, int& cluster, int& subgraph, std::string& inp, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);

void entry_end_handler (int& subgraph, int& k, std::string& inp, int len, int& basic_block, 
                        std::string& code, std::vector <std::vector<std::string>>& Code);

void vertex_handler    (std::string& code, int& basic_block, size_t& found1, int len, std::string& inp, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);

void edges_handler     (int& found1, size_t& found2, std::string& inp, std::vector <std::vector<std::pair<std::string, int>>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int& subgraph);

void blocks_handler    (std::string& code, int& basic_block, int& cluster, int& subgraph, 
                        std::string& inp, std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::pair<std::string, int>>>& V, std::vector <std::vector<std::string>>& Code, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, int len);

void code_handler      (std::string& inp, int& basic_block, std::string& code, 
                        std::vector <std::vector<std::string>>& Code, int& subgraph);

void graph_list        (std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters);

void vertex_list       (std::vector <std::vector<std::pair<std::string, int>>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E, 
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters, 
                        std::vector <std::vector<std::string>>& Code);

void adjacency_list    (std::vector <std::vector<std::pair<std::string, int>>>& V, 
                        std::vector <std::vector<std::vector<std::pair<int, std::string>>>>& E);
                        


void new_graph_list    (std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters,
                        std::vector <std::vector<std::pair<std::string, int>>>& V);

void new_vertex_list   (std::vector <std::vector<std::pair<std::string, int>>>& V,
                        std::vector <std::pair<std::string, std::pair<int, int>>>& Clusters,
                        std::vector<std::vector<int>>& V_new, std::vector<std::string>& Code_new);

void new_adjacency_list(std::vector<std::vector<std::pair<int, std::string>>>& E_new);

void input_V_E         (std::ifstream& fin, std::vector<std::vector<std::pair<int, std::string>>>& E_new, 
                        std::vector<std::pair<std::string, std::string> >& rules, std::ifstream& input_file);

void visualising_graph (std::string& path_to_visualizator, std::vector<std::vector<int>>& V_new, std::vector<std::vector<std::pair<int, std::string>>>& E_new,	std::vector<std::string>& Code_new, std::vector <std::vector<std::pair<std::string, int>>>& V);

void to_fifo           (std::string bin_path, std::vector<std::vector<int>>& V_new, 
                        std::vector <std::vector<std::pair<std::string, int>>>& V,
                        std::vector<std::vector<std::pair<int, std::string>>>& E_new, 
                        std::vector<std::pair<std::string, std::string> >& rules);

int number_of_file_arg(int argc, char* argv[], const char* arg);
int get_input_type(int argc, char* argv[]);