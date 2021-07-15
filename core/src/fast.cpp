#include "fast.h"

int main(int argc, char* argv[]) 
{
    int m, number, E, initial, P, V;
    std::vector <std::string> nonterminals;
    std::vector <std::vector<int>> lol(NUMBER_OF_LETTERS_WITH_OVERFLOW);
    std::vector <rule> eps_rules, rules;
    std::string bin_path = argv[1];
    std::ifstream fin(bin_path + "../data/graph");
    if (!fin.is_open()) {
        std::cout << bin_path + "../data/graph" << " was not opened" << std::endl;
        return -1;
    }
    
    fin >> number >> m;
    for (int i = 0; i < m; i++)
        input_rules(initial, eps_rules, lol, fin, nonterminals, rules);

    std::vector <std::vector<int>> left_rules(nonterminals.size()), right_rules(nonterminals.size());
    for (int i = 0; i < rules.size(); i++)
        if (rules[i].type == 1)
            left_rules[rules[i].right1[0]].push_back(i), right_rules[rules[i].right1[1]].push_back(i);
    
    fin >> V >> E; //input number of vertexes and edges
    std::vector<std::string> nodes_names(V);
    for (int i = 0; i < V; i++)
        fin >> nodes_names[i];
        
    P = (V != 1) ? ceil(log10(V)): 1; //defining the length of binary number

    std::vector <std::pair <int, std::pair <int, std::string> > > edges;
    std::deque <std::vector<int>> W;
    std::vector<unsigned int> a = new_fastset(P, V);
    std::vector <std::vector <std::vector <unsigned int> > > H1(nonterminals.size(), std::vector<std::vector <unsigned int > > (V, a)), H2(nonterminals.size(), std::vector<std::vector <unsigned int> > (V, a));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(P, fin, lol, W, H1, H2);

    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1, H2); //if word is empty
    for (int i = 0; i < H1.size(); i++)
    {
        for (int j = 0; j < H1[i].size(); j++)
        {
            std::cout << i << " " << nonterminals[i] <<  " " << j << "\n";
            for (int q = 0; q < H1[i][j].size(); q++)
            {
                std::cout << H1[i][j][q] <<  " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "--------------------\n";
    for (int i = 0; i < H2.size(); i++)
    {
        for (int j = 0; j < H2[i].size(); j++)
        {
            std::cout << i << " " << nonterminals[i] <<  " " << j << "\n";
            for (int q = 0; q < H2[i][j].size(); q++)
            {
                std::cout << H2[i][j][q] <<  " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    while (!W.empty()) {
        std::vector<int> q = W.front();
        W.pop_front();
        baseline_cfl(0, q[0], q[2], H2, right_rules, q[1], P, V, rules, prev, W, H1, H2);
        baseline_cfl(1, q[2], q[0], H1, left_rules, q[1], P, V, rules, prev, W, H1, H2);
    }

    output(P, V, initial, prev, H1);
    fin.close();
}