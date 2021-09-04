#include "core.h"
int main(int argc, char* argv[]) 
{
    bool is_fast = (find_arg(argc, argv, "-fast") > 0) ? true : false;
    void* sl = dlopen((is_fast == true) ? "libfst.so" : "libslw.so", RTLD_LAZY);
    std::cout << ((is_fast == true) ? "libfst.so" : "libslw.so") << std::endl;
    funcs func(sl);
    if (check_funcs(func) < 0)
        return -1;
    int m, number, E, initial, P, V;
    std::vector <std::string> nonterminals;
    std::vector <std::vector<int>> lol(NUMBER_OF_LETTERS_WITH_OVERFLOW);
    std::vector <rule> eps_rules, rules;
    std::string path_to_graph = argv[1];
    std::ifstream fin(path_to_graph);
    if (!fin.is_open()) {
        std::cout << path_to_graph << " was not opened" << std::endl;
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

    P = func.create_P(V);
    std::vector<std::pair<int, std::pair<int, std::string> > > edges;
    std::deque<std::vector<int> > W;
    std::vector<std::vector<std::vector<unsigned int>>>H1v = func.create_Hv(nonterminals.size(), V, P), H2v = H1v;
    std::vector<std::vector<std::unordered_set<int>>>H1u = func.create_Hu(nonterminals.size(), V, P), H2u = H1u;
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(P, fin, lol, W, H1v, H2v, H1u, H2u, func.add_value);
    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1v, H2v, H1u, H2u, func.add_value); //if word is empty
    while (!W.empty()) {
        std::vector<int> q = W.front();
        W.pop_front();
        baseline_cfl(is_fast, 0, q[0], q[2], H2v, H2u, right_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
        baseline_cfl(is_fast, 1, q[2], q[0], H1v, H1u, left_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
    }
    output(is_fast, P, V, initial, prev, H1v, H1u, func.create_q);
    fin.close();
}