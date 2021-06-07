// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar,
// next line path
#include "CYKonGraph.h"

int main()
{
    std::vector<rule> rules;
    std::vector<int> topsort;
    std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
    std::ifstream fin;
    int initial, tests;
    fin.open("../data/graph");
    fin >> tests;
    for (int test = 0, m; test < tests; test++, rules.clear(), topsort.clear(), last.clear())
    {
        //grammar
        std::vector<std::string> nonterminals;
        fin >> m; //quantity of rules
        for (int i = 0, num; i < m; i++)
        {
            std::string left, right;
            fin >> left >> right;
            if (right.size() == 1 || right.size() == 2)
                num = right.size();

            if (num == 1)
            {
                std::string a_1 = left, a_2 = right;
                int FLAG  = check(nonterminals, a_1, initial);
                if (FLAG == nonterminals.size())
                    nonterminals.push_back(a_1);
                rules.push_back({0, FLAG, {0}, a_2});
            }
            else if (num == 2)
            {
                std::string a_1 = left, a_2, a_3;
                a_2 = right[0];
                a_3 = right[1];
                int FLAG1 = check(nonterminals, a_1, initial), FLAG2  = check(nonterminals, a_2, initial), FLAG3  =  check(nonterminals, a_3, initial);
                if (FLAG1 == nonterminals.size())
                    nonterminals.push_back(a_1);
                if (FLAG2 == nonterminals.size())
                    nonterminals.push_back(a_2);
                if (FLAG3 == nonterminals.size())
                    nonterminals.push_back(a_3);
                rules.push_back({1, FLAG1, {FLAG2, FLAG3}});
            }
        }
        int V, E;
        fin >> V >> E;
        std::vector<std::pair<int, std::pair<int, std::string> > > edges;
        std::vector <std::string> V_names(V);
        for (int i = 0; i < V; i++)
            fin >> V_names[i];
        for (int i = 0; i < E; i++)	
        {
            int u1, u2;
            std::string a;
            fin >> u1 >> u2 >> a;
            std::pair<int, std::pair<int, std::string> > v1;
            v1.first = u1;
            v1.second.first = u2;
            v1.second.second = a;
            edges.push_back(v1);
        }
        std::pair<int, std::pair<int, int>> y;
        y.first = -1, y.second.first = -1, y.second.second = -1;
        std::vector<std::vector<std::pair<int, std::pair<int, int> > > > l (V, std::vector<std::pair<int, std::pair<int, int> > > (V, y));
        for (int i = 0; i < nonterminals.size(); i++)
            last.push_back(l);
    // Finding RTDG(rules)
        std::vector<std::vector<int> > RTDG;
        find_rtdg(RTDG, nonterminals, topsort, rules);
    // Semi-Naive CFL
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;

        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
        {
            std::vector<std::string> A;
            std::vector<std::vector<int> > G_A(V, std::vector<int>(V, 0));
            std::vector<std::vector<std::string> > G_L(V, std::vector<std::string>(V));
            for (auto j : rules)
            {
                if (j.type == 0 && j.left == i)
                {
                    A.push_back(j.right0);
                    if (j.right0 == "0")
                        eps = 1;
                }

            }
            for (auto j : edges)
            {
                for (auto u : A)
                {
                    if (j.second.second == u)
                    {
                        G_A[j.first][j.second.first] = 1;
                        G_L[j.first][j.second.first] = u;
                        std::pair<int, std::pair<int, int> > u;
                        u.first = -2;
                        u.second.first = -2;
                        u.second.second = -2;
                        last[i][j.first][j.second.first] = u;

                    }
                }
            }
            if (eps == 1)
            {
                for (int j = 0; j < V; j++)
                {
                    G_A[j][j] = 1;
                    G_L[j][j] = "0";
                    std::pair<int, std::pair<int, int> > u;
                    u.first = -2;
                    u.second.first = -2;
                    u.second.second = -2;
                    last[i][j][j] = u;
                }
            }
            g.push_back(G_A);
            g_l.push_back(G_L);

        }
        std::vector<std::vector<std::vector<int> > > delta;
        delta = g;
        for (int k = RTDG.size() - 1, flag = 1; k >= 0; k--, flag = 1)
        {
            while (flag)
            {
                flag = 0;
                for (auto i : RTDG[k])
                {
                    if (not_null(delta[i]))
                    {
                        flag = 1;
                        std::vector<std::vector<int> > delta_I;
                        delta_I = delta[i];
                        delta[i] = razn(delta[i], delta[i]);
                        for (auto h: rules)
                        {
                            if (h.type == 1 && (h.right1[0] == i || h.right1[1] == i)) {
                                std::vector<std::vector<int>> temp;
                                temp = (h.right1[0] == RTDG[k][i]) ? mult(delta_I, g[h.right1[1]], h.left, i, h.right1[1], last) : mult(g[h.right1[0]], delta_I, h.left, h.right1[0], i, last);
                                delta[h.left] = sum(delta[h.left], temp);
                                g[h.left] = sum(g[h.left], delta[h.left]);
                            }
                        }
                    }
                }
            }

        }
        int ans = 0;
        for (auto i : g[initial])
            for (auto j : i)
                if (j == 1)
                    ans++;
        print_results(test, ans, g, V_names, g_l, initial, last);
    }
    fin.close();
}