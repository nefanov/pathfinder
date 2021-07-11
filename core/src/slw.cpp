#include "slow.h"

int check(int& initial, std::vector<std::string>& a, std::string b)
{
    for (int i = 0; i < a.size(); i++) {
        if (a[i] == b) {
            if (b == "S")
                initial = i;
            return i;
        }
    }
    if (b == "S")
        initial = a.size();
    return a.size();
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

int filling_edge_matrices(std::ifstream& fin, std::vector <std::vector<int>>& lol, std::deque <std::vector<int>>& W, std::vector <std::vector <std::unordered_set <int> > >& H1, std::vector <std::vector <std::unordered_set <int> > >& H2)
{
    int u1, u2, s = 0;
    std::string str;
    fin >> u1 >> u2 >> str;
    if (str != "0") {
        s = str[0] - 'a';
    }
    for (auto j: lol[s]) {
        W.push_back({u1, j, u2});
        H1[j][u1].insert(u2);
        H2[j][u2].insert(u1);
    }
    return 0;
}

int filling_loops(int j, int P, rule& i, std::deque <std::vector<int>>& W, std::vector <std::vector <std::unordered_set <int> > >& H1, std::vector <std::vector <std::unordered_set <int> > >& H2)
{
    W.push_back({i.left, j, i.left});
    H1[j][i.left].insert(i.left);
    H2[j][i.left].insert(i.left);
    return 0;
}

int baseline_cfl(int flag, int i2, int i3, std::vector <std::vector <std::unordered_set <int> > >& Hi, std::vector<std::vector<int>> side_rules, int B, std::vector <rule>& rules, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::deque <std::vector<int>>& W, std::vector <std::vector <std::unordered_set <int> > >& H1, std::vector <std::vector <std::unordered_set <int> > >& H2)
{
    //flag: 0  1
    //Hi  : H2 H1
    //i2  : u  v
    //i3  : v  u
    //i4  : v  d
    //i5  : d  u
    for (auto i:side_rules[B]) {
        int C = rules[i].right1[flag], A = rules[i].left, i4, i5;
        std::unordered_set<int> w = Hi[C][i2];
        for (auto d: Hi[A][i3])
            w.erase(d);
        for (auto d: w) {
            (flag) ? (i4 = d, i5 = i3) : (i4 = i3, i5 = d);
            H2[A][i4].insert(i5), H1[A][i5].insert(i4);
            W.push_back({i5, A, i4});
            if (prev[i5][A][i4][0] == -1)
                prev[i5][A][i4] = {i2, C, B};
        }
    }
    return 0;
}


std::vector<int> path_find(int i, int j, int nonterm,  std::vector<std::vector<std::vector<std::vector<int> > > >& prev)
{
    std::vector<int> u = prev[i][nonterm][j];
    if (u[0] == -1 )
        return {i, j};
    std::vector<int> left = path_find(i, u[0], u[1], prev), right = path_find(u[0], j, u[2],  prev), way = left;
    way.pop_back();
    for (auto k: right)
        way.push_back(k);
    return way;
}


int output(int V, int initial, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::vector <std::vector <std::unordered_set <int> > >& H1)
{
    int counter = 0;
    for (int i = 0; i < V; i++) {
        for (auto j: H1[initial][i]) {
            counter++;
            std::cout << i << " " << j << std::endl;
            std::vector<int> way = path_find(i, j, initial,  prev);
            for (auto k: way)
                std::cout << k << " ";
            std::cout << std::endl;
        }
    }
    std::cout << counter << std::endl;
    return 0;
}