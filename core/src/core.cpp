#include "core.h"
static struct funcs* func;

int main(int argc, char* argv[]) 
{
    void* sl = dlopen(((strcmp(argv[2], "-fast") == 0) ? "lfst.so" : "lslw.so"), RTLD_LAZY);
    
    
    struct funcs func_real = {
        reinterpret_cast<void(*)(std::unordered_set<unsigned int> &u, std::vector<unsigned int> &v, int a, int P)>(dlsym(sl, "add_value")), 
        reinterpret_cast<std::vector<unsigned int>(*)(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)>(dlsym(sl, "difference")), 
        reinterpret_cast<std::vector<int>(*)(int P, std::vector<unsigned int> kek)>(dlsym(sl, "not_null")), 
        reinterpret_cast<std::vector<int>(*)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2)>(dlsym(sl, "create_w")), 
        reinterpret_cast<std::vector<int>(*)(int P, std::vector<unsigned int> v)>(dlsym(sl, "create_q")), 
        reinterpret_cast<std::vector <std::vector <std::vector <unsigned int> > >(*)(int ntsize, int V, int P)>(dlsym(sl, "create_H")), 
        reinterpret_cast<int(*)(int V)>(dlsym(sl, "create_P"))
    };
    func = &func_real;

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
    
    P = func->create_P(V);
    std::vector<std::pair<int, std::pair<int, std::string> > > edges;
    std::deque<std::vector<int> > W;
    auto H1 = func->create_H(nonterminals.size(), V, P), H2 = H1;
    //std::vector<std::vector<std::unordered_set<int> > > H1 (nonterminals.size(), std::vector<std::unordered_set<int> > (V)), H2 (nonterminals.size(), std::vector<std::unordered_set<int> > (V));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(P, fin, lol, W, H1, H2, func->add_value);

    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1, H2, func->add_value); //if word is empty

    while (!W.empty()) {
        std::vector<int> q = W.front();
        W.pop_front();
        baseline_cfl(0, q[0], q[2], H2, right_rules, q[1], P, V, rules, prev, W, H1, H2, func->add_value, func->create_w);
        baseline_cfl(1, q[2], q[0], H1, left_rules, q[1], P, V, rules, prev, W, H1, H2, func->add_value, func->create_w);
    }
    output(P, V, initial, prev, H1, func->create_q);
    fin.close();
}