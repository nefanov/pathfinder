#include "fast.h"

std::vector<unsigned int> new_fastset(int P, int V)
{
    int k = (V % P == 0)? (V / P): (V / P) + 1;
    std::vector<unsigned int> fset(k, 0);
    return fset;
}
void add_value(std::vector<unsigned int> &kek, int a, int P)
{
    kek[a / P] |= (1 << (a % P));
}       

std::vector<unsigned int> difference(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)
{
    std::vector<unsigned int> ans = new_fastset(P, V);
    for (int i = 0; i < kek1.size(); i++)
    {
        unsigned int s1 = kek2[i], s = ~s1;
        s &= ((1 << P) - 1);
        ans[i] = (kek1[i] & (s));
    }
    return ans;
}
std::vector<int> not_null(int P, std::vector<unsigned int> &kek)
{
    std::vector<int> ans;
    for (int i = 0; i < kek.size(); i++)
    {
        if (kek[i] != 0)
        {
            unsigned int m = kek[i];
            for (int j = 0; j < P; j++, m /= 2)
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
int check(int& initial, std::vector<std::string>& a, std::string b)
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


int output(int P, int V, int initial, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::vector <std::vector <std::vector <unsigned int> > >& H1)
{
    std::cout << P << std::endl;
    int counter = 0;
    for (int i = 0; i < V; i++)
    {
        std::vector<int> q = not_null(P, H1[initial][i]);
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
    return 0;
}

int input_rules(int& initial, std::vector <rule>& eps_rules, std::vector <std::vector<int>>& lol, std::ifstream& fin, std::vector <std::string>& nonterminals, std::vector <rule>& rules)
{
    std::string left, right;
    fin >> left >> right;
    int num = right.size();
    if (right.size() != 2 && right.size() != 1) {
        std::cout << "right.size() != 2 && right.size() != 1" << std::endl;
        return -1;
    }
    if (num == 1) {
        int FLAG = check(initial, nonterminals, left);
        if (FLAG == nonterminals.size())
            nonterminals.push_back(left);
        if (right != "0")
            lol[right[0] - 'a'].push_back(FLAG); //if rules can be raped..
        else
            lol[0].push_back(FLAG), eps_rules.push_back({0, FLAG, {0, 0}, right});
        rules.push_back({0, FLAG, {0, 0}, right});
    } else {
        std::string a_2, a_3;//, a_2(right[0]), a_3(right[1]);
        a_2 = right[0], a_3 = right[1];
        int FLAG1 = check(initial, nonterminals, left), FLAG2, FLAG3;
        if (FLAG1 == nonterminals.size())
            nonterminals.push_back(left);
        FLAG2 = check(initial, nonterminals, a_2);
        if (FLAG2 == nonterminals.size())
            nonterminals.push_back(a_2);
        FLAG3 = check(initial, nonterminals, a_3);
        if (FLAG3 == nonterminals.size())
            nonterminals.push_back(a_3);
        rules.push_back({1, FLAG1, {FLAG2, FLAG3}, {0, 0}});
    }
    return 0;
}

int filling_loops(int j, int P, rule& i, std::deque <std::vector<int>>& W, std::vector <std::vector <std::vector <unsigned int> > >& H1, std::vector <std::vector <std::vector <unsigned int> > >& H2)
{
    W.push_back({i.left, j, i.left});
    add_value(H1[j][i.left], i.left, P), add_value(H2[j][i.left], i.left, P);
    return 0;
}

int filling_edge_matrices(int P, std::ifstream& fin, std::vector <std::vector<int>>& lol, std::deque <std::vector<int>>& W, std::vector <std::vector <std::vector <unsigned int> > >& H1, std::vector <std::vector <std::vector <unsigned int> > >& H2)
{
    int u1, u2, s = 0;
    std::string str;
    fin >> u1 >> u2 >> str;
    if (str != "0")
        s = str[0] - 'a';
    for (auto j: lol[s]) {
        W.push_back({u1, j, u2});
        add_value(H1[j][u1], u2, P), add_value(H2[j][u2], u1, P);
    }
    return 0;
}