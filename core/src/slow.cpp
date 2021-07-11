#include "slow.h"

int main(int argc, char* argv[]) {
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
    
    std::vector<std::pair<int, std::pair<int, std::string> > > edges;
    std::deque<std::vector<int> > W;
    std::vector<std::vector<std::unordered_set<int> > > H1 (nonterminals.size(), std::vector<std::unordered_set<int> > (V)), H2 (nonterminals.size(), std::vector<std::unordered_set<int> > (V));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(fin, lol, W, H1, H2);

    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1, H2); //if word is empty

    while (!W.empty())
    {
        std::vector<int> q = W.front();
        W.pop_front();
        baseline_cfl(0, q[0], q[2], H2, right_rules, q[1], rules, prev, W, H1, H2);
        baseline_cfl(1, q[2], q[0], H1, left_rules, q[1], rules, prev, W, H1, H2);
    }
    output(V, initial, prev, H1);
    fin.close();
}