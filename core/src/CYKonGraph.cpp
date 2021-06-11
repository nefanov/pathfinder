// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar,
// next line path
#include "CYKonGraph.h"

int main(int argc, char* argv[])
{
    int initial, tests;
    std::ifstream fin;
    std::string bin_path = argv[0];
    fin.open(bin_path + "../data/graph");
    fin >> tests;
    for (int test = 0, m, V, E; test < tests; test++) {
        std::vector<rule> rules;
        std::vector<int> topsort;
        std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
        std::vector<std::string> nonterminals;
        std::vector<std::vector<int> > RTDG;
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;
        fin >> m; //quantity of rules
        for (int i = 0; i < m; i++)
            input_rules(i, nonterminals, rules, fin, initial);
        fin >> V >> E;
        std::vector<std::pair<int, std::pair<int, std::string> > > edges;
        std::vector<std::string> V_names(V);
        std::vector<std::vector<std::pair<int, std::pair<int, int> > > > l (V, std::vector<std::pair<int, std::pair<int, int> > > (V, {-1, {-1, -1}}));
        input_V_names(V_names, edges, fin, V, E);
        for (int i = 0; i < nonterminals.size(); i++)
            last.push_back(l);
        find_rtdg(RTDG, nonterminals, topsort, rules);
        // Semi-Naive CFL
        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
            arranging_rules_to_edges(i, eps, edges, g, g_l, initial, last, rules , V);
        std::vector<std::vector<std::vector<int> > > delta = g;
        for (int k = RTDG.size() - 1, flag = 1; k >= 0; flag = 1, k--)
            for (flag; flag; flag = 0)
                for (auto i : RTDG[k])
                    if (not_null(delta[i]))
                        transitive_closure(flag, delta, RTDG, g, i, k, last, rules);     
        print_results(test, count_ans(g, initial), g, V_names, g_l, initial, last);
    }
    fin.close();
}