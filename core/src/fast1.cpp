#include "fast.h"



int main(int argc, char* argv[]) {
    int m, number, E, initial, P, V;
    std::vector <std::string> nonterminals;
    std::vector <std::vector<int>> lol(NUMBER_OF_LETTERS_WITH_OVERFLOW);
    std::vector <rule> eps_rules, rules;
    std::string bin_path = argv[1], str;
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
    
    fin >> V >> E;
    for (int i = 0; i < V; i++)
        fin >> str; //skip nodes' names
        
    P = ceil(log10(V));
    if (P == 0)
        P += 1;
    if (P > V)
        P = V;
    std::vector <std::pair <int, std::pair <int, std::string> > > edges;
    std::deque <std::vector<int>> W;
    std::vector<unsigned int> a = new_fastset(P, V);
    std::vector <std::vector <std::vector <unsigned int> > > H1(nonterminals.size(), std::vector<std::vector <unsigned int > > (V, a)), H2(nonterminals.size(), std::vector<std::vector <unsigned int> > (V, a));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
    {
        int u1, u2, s = 0;
        fin >> u1 >> u2 >> str;
        if (str != "0")
            s = str[0] - 'a';
        for (auto j: lol[s]) {
            W.push_back({u1, j, u2});
            add_value(H1[j][u1], u2, P), add_value(H2[j][u2], u1, P);
        }
    }

    for (auto i: eps_rules)
    {
        for (int j = 0; j < V; j++)
        {
            W.push_back({i.left, j, i.left});
            add_value(H1[j][i.left], i.left, P), add_value(H2[j][i.left], i.left, P);
        }
    }

    while (!W.empty())
    {
        std::vector<int> q = W.front();
        W.pop_front();
        int B = q[1], u = q[0], v = q[2];
        for (auto i:right_rules[B])
        {
            int C = rules[i].right1[0], A = rules[i].left;
            std::vector<unsigned int> w1 = H2[C][u], w2 = H2[A][v], w3 = difference(P, V, w1, w2);
            std::vector<int> l = not_null(P, w3);
            for (auto d: l)
            {
                add_value(H2[A][v], d, P), add_value(H1[A][d], v, P);
                W.push_back({d, A, v});
                if (prev[d][A][v][0] == -1)
                    prev[d][A][v][0] = u, prev[d][A][v][1] = C, prev[d][A][v][2] = B;
            }

        }

        for (auto i:left_rules[B])
        {
            int C = rules[i].right1[1], A = rules[i].left;
            std::vector<unsigned int> w1 = H1[C][v], w2 = H1[A][u], w3 = difference(P, V, w1, w2);
            std::vector<int> l = not_null(P, w3);
            for (auto d: l)
            {
                add_value(H2[A][d], u, P), add_value(H1[A][u], d, P);
                W.push_back({u, A, d});
                if (prev[u][A][d][0] == -1)
                    prev[u][A][d][0] = v, prev[u][A][d][1] = B, prev[u][A][d][2] = C;
            }
        }
    }
    output(P, V, initial, prev, H1);
    fin.close();
}