#include "core.h"
int main(int argc, char* argv[]) 
{
    bool is_fast = (argc >= 3 && strcmp(argv[2], "-fast") == 0) ? true : false;
    void* sl = dlopen(((is_fast == true) ? "libfst.so" : "libslw.so"), RTLD_LAZY);
    if (sl == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }  
    std::cout << ((is_fast) ? "FAST" : "SLOW") << std::endl;
    std::cout << "10\n";
    if (dlsym(sl, "add_value") == NULL) {
        fprintf(stderr, "%s\n", dlerror());
        return -1;
    }  
    funcs func(
        dlsym(sl, "add_value"), 
        dlsym(sl, "difference"), 
        dlsym(sl, "not_null"), 
        dlsym(sl, "create_wv"), 
        dlsym(sl, "create_wu"),
        dlsym(sl, "create_q"), 
        dlsym(sl, "create_Hv"),
        dlsym(sl, "create_Hu"),
        dlsym(sl, "create_P")
    );

    std::cout << "21\n";
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
    std::cout << "32\n";
    
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
    std::cout << "47\n";
    
    P = func.create_P(V);
    std::cout << "50\n";

    std::vector<std::pair<int, std::pair<int, std::string> > > edges;
    std::deque<std::vector<int> > W;
    std::cout << "52\n";

    //auto H1 = func.create_H(nonterminals.size(), V, P), H2 = H1;
    std::vector<std::vector<std::vector<unsigned int>>>H1v = func.create_Hv(nonterminals.size(), V, P), H2v = H1v;
    std::vector<std::vector<std::unordered_set<int>>>H1u = func.create_Hu(nonterminals.size(), V, P), H2u = H1u;
    std::cout << "55\n";

    //std::vector<std::vector<std::unordered_set<int> > > H1 (nonterminals.size(), std::vector<std::unordered_set<int> > (V)), H2 (nonterminals.size(), std::vector<std::unordered_set<int> > (V));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(P, fin, lol, W, H1v, H2v, H1u, H2u, func.add_value);
    std::cout << "60\n";

    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1v, H2v, H1u, H2u, func.add_value); //if word is empty
    std::cout << "65\n";

    while (!W.empty()) {
        std::vector<int> q = W.front();
        W.pop_front();
    std::cout << "81\n";

        baseline_cfl(is_fast, 0, q[0], q[2], H2v, H2u, right_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
    std::cout << "84\n";
        
        baseline_cfl(is_fast, 1, q[2], q[0], H1v, H1u, left_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
    std::cout << "87\n";
    
    }
    std::cout << "85\n";
    std::cout << "H111111:\n";
    for (auto i:H1u) {
        for (auto j:i) {
            for (auto k:j)
                std::cout << k << " ";
            std::cout<<std::endl;
    }
            std::cout<<std::endl;


    }
            std::cout<<std::endl;
            std::cout<<std::endl;
            std::cout<<std::endl;

    output(is_fast, P, V, initial, prev, H1v, H1u, func.create_q);
    fin.close();
}