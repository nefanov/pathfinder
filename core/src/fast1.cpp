#include "fast.h"
int initial, P, V;

std::vector<unsigned int> new_fastset()
{
    int k = (V % P == 0)? (V / P): (V / P) + 1;
    std::vector<unsigned int> fset(k, 0);
    return fset;
}
void add_value(std::vector<unsigned int> &kek, int a)
{
    kek[a / P] |= (1 << (a % P));
}       

std::vector<unsigned int> difference(std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)
{
    std::vector<unsigned int> ans = new_fastset();
    for (int i = 0; i < kek1.size(); i++)
    {
        unsigned int s1 = kek2[i], s = ~s1;
        s &= ((1 << P) - 1);
        ans[i] = (kek1[i] & (s));
    }
    return ans;
}
std::vector<int> not_null(std::vector<unsigned int> &kek)
{
    std::vector<int> ans;
    for (int i = 0; i < kek.size(); i++)
    {
        if (kek[i] != 0)
        {
            unsigned int m = kek[i];
            for (int j = 0; j < P; j++, m/=2)
                if (m % 2 == 1)
                    ans.push_back(j + i * P);
        }
    }
    return ans;
}
std::vector<int> path_find(int i, int j, int nonterm, std::vector<std::vector<std::vector<std::vector<int> > > >& prev)
{
    std::vector<int> u = prev[i][nonterm][j];
    if (u[0] == -1)
    {
        std::vector<int> kek = {i, j};
        return kek;
    }
    std::vector<int> left = path_find(i, u[0], u[1], prev), right = path_find(u[0], j, u[2],  prev), way = left;
    way.pop_back();
    for (auto k: right)
        way.push_back(k);
    return way;
}
int check(std::vector<std::string>& a, std::string b)
{
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == b)
        {
            if (b == "S")
                initial = i;
            return i;
        }
    }
    if (b == "S")
        initial = a.size();
    return a.size();
}
int triple_hash(int a1, int a2, int a3, int len1, int len2, int len3)
{
    return a3 + len3 * a2 + len3 * len2 * a1;
}
std::vector<rule> rules;

int main(int argc, char* argv[]) {
    //grammar
    int m, number, E; //quantity of rules
    std::vector <std::string> nonterminals;
    std::vector <std::vector<int>> lol(NUMBER_OF_LETTERS_WITH_OVERFLOW);
    std::vector <rule> eps_rules;
    std::string bin_path = argv[1], str;
    std::ifstream fin(bin_path + "../data/graph");
    if (!fin.is_open()) {
        std::cout << bin_path + "../data/graph" << " was not opened\n";
    }
    fin >> number >> m;
    for (int i = 0, num = 0; i < m; i++) {
        std::string left, right;
        fin >> left >> right;
        if (right.size() == 2)
            num = 2;
        else if (right.size() == 1)
            num = 1;
        if (num == 1) {
            std::string a_1 = left, a_2 = right;
            int FLAG = check(nonterminals, a_1);
            if (FLAG == nonterminals.size())
                nonterminals.push_back(a_1);
            if (a_2 != "0")
                lol[a_2[0] - 'a' + 1].push_back(FLAG); //if rules can be raped..
            else
                lol[0].push_back(FLAG), eps_rules.push_back({0, FLAG, {0, 0}, a_2});
            rules.push_back({0, FLAG, {0, 0}, a_2});
        } else if (num == 2) {
            std::string a_1 = left, a_2, a_3;//, a_2(right[0]), a_3(right[1]);
            a_2 = right[0], a_3 = right[1];
            int FLAG1 = check(nonterminals, a_1), FLAG2, FLAG3;
            if (FLAG1 == nonterminals.size())
                nonterminals.push_back(a_1);
            FLAG2 = check(nonterminals, a_2);
            if (FLAG2 == nonterminals.size())
                nonterminals.push_back(a_2);
            FLAG3 = check(nonterminals, a_3);
            if (FLAG3 == nonterminals.size())
                nonterminals.push_back(a_3);
            rules.push_back({1, FLAG1, {FLAG2, FLAG3}, {0, 0}});
        }
    }
    std::vector <std::vector<int>> left_rules(nonterminals.size()), right_rules(nonterminals.size());
    for (int i = 0; i < rules.size(); i++) {
        if (rules[i].type == 1) {
            int u = rules[i].right1[0], v = rules[i].right1[1];
            left_rules[u].push_back(i), right_rules[v].push_back(i);
        }
    }
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
    std::vector<unsigned int> a = new_fastset();
    std::vector <std::vector <std::vector <unsigned int> > > H1(nonterminals.size(), std::vector<std::vector <unsigned int > > (V, a)), H2(nonterminals.size(), std::vector<std::vector <unsigned int> > (V, a));
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
    {
        int u1, u2, s = 0;
        fin >> u1 >> u2 >> str;
        if (str != "0")
            s = str[0] - 'a' + 1;
        for (auto j: lol[s]) {
            std::vector<int> z = {u1, j, u2};
            W.push_back({u1, j, u2});
            add_value(H1[j][u1], u2), add_value(H2[j][u2], u1);
        }
    }

    for (auto i: eps_rules)
    {
        for (int j = 0; j < V; j++)
        {
            std::vector<int> z = {i.left, j, i.left};
            W.push_back({i.left, j, i.left});
            add_value(H1[j][i.left], i.left), add_value(H2[j][i.left], i.left);
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
            std::vector<unsigned int> w1 = H2[C][u], w2 = H2[A][v], w3 = difference(w1, w2);
            std::vector<int> l = not_null(w3);
            for (auto d: l)
            {
                add_value(H2[A][v], d), add_value(H1[A][d], v);
                std::vector<int> z = {d, A, v};
                W.push_back(z);
                if (prev[d][A][v][0] == -1)
                    prev[d][A][v][0] = u, prev[d][A][v][1] = C, prev[d][A][v][2] = B;
            }

        }

        for (auto i:left_rules[B])
        {
            int C = rules[i].right1[1], A = rules[i].left;
            std::vector<unsigned int> w1 = H1[C][v], w2 = H1[A][u], w3 = difference(w1, w2);
            std::vector<int> l = not_null(w3);
            for (auto d: l)
            {
                add_value(H2[A][d], u), add_value(H1[A][u], d);
                std::vector<int> z = {u, A, d};
                W.push_back(z);
                if (prev[u][A][d][0] == -1)
                    prev[u][A][d][0] = v, prev[u][A][d][1] = B, prev[u][A][d][2] = C;
            }
        }
    }
    std::cout << P << std::endl;
    int counter = 0;
    for (int i = 0; i < V; i++)
    {
        std::vector<int> q = not_null(H1[initial][i]);
        for (auto j: q)
        {
            counter += 1;
            std::cout << i << " " << j << std::endl;
            std::vector<int> way = path_find(i, j, initial, prev);
            for (auto k: way)
                std::cout << k << " ";
            std::cout << std::endl;
        }
    }
    std::cout << counter << std::endl;
    fin.close();
}