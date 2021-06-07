// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar,
// next line path
#include "CYKonGraph.h"

int main()
{
    std::ifstream fin;
    int initial, tests, ans;
    fin.open("../data/graph");
    fin >> tests;
    for (int test = 0, m, V, E; test < tests; ans = 0, test++) {
        std::vector<rule> rules;
        std::vector<int> topsort;
        std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
        std::vector<std::string> nonterminals;
        fin >> m; //quantity of rules
        for (int i = 0; i < m; i++)
            input(i, nonterminals, rules, fin, initial);
        fin >> V >> E;
        std::vector<std::pair<int, std::pair<int, std::string> > > edges;
        std::vector <std::string> V_names(V);
        for (int i = 0; i < V; i++)
            fin >> V_names[i];
        for (int i = 0, u1, u2; i < E; i++)	{
            std::string a;
            fin >> u1 >> u2 >> a;
            edges.push_back({u1, {u2, a}});
        }
        std::pair<int, std::pair<int, int>> y = {-1, {-1, -1}};
        std::vector<std::vector<std::pair<int, std::pair<int, int> > > > l (V, std::vector<std::pair<int, std::pair<int, int> > > (V, y));
        for (int i = 0; i < nonterminals.size(); i++)
            last.push_back(l);
        std::vector<std::vector<int> > RTDG;
        find_rtdg(RTDG, nonterminals, topsort, rules);
        // Semi-Naive CFL
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;
        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
            arranging_rules_to_edges(i, eps, edges, g, g_l, initial, last, rules , V);
        std::vector<std::vector<std::vector<int> > > delta = g;
        for (int k = RTDG.size() - 1, flag = 1; k >= 0; k--, flag = 1) //
            while (flag = 0)
                for (auto i : RTDG[k])
                    if (not_null(delta[i]))
                        transitive_closure(flag, delta, RTDG, g, i, k, last, rules);
        for (auto i : g[initial])
            for (auto j : i)
                if (j == 1)
                    ans++;
        print_results(test, ans, g, V_names, g_l, initial, last);
    }
    fin.close();
}