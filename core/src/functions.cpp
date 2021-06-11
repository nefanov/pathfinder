#include "CYKonGraph.h"

int not_null(std::vector<std::vector<int> >& A) //function for checking if A is zero
{
    for (auto i : A)
        for (auto j : i)
            if (j == 1)
                return 1;
    return 0;
}

std::vector<std::vector<int> > mult(std::vector<std::vector<int> >& A,  std::vector<std::vector<int> >& B, int num, int numA, int numB, std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > >& last)
{
    std::vector<std::vector<int> > C(A.size(), std::vector<int> (B.size(), 0));
    for (int g = 0; g < A.size(); g++)
    {
        for (int v = 0, kek = 0; v < B.size(); v++, kek = 0)
        {
            for (int s = 0; s < A.size(); s++)
            {
                kek += A[g][s] * B[s][v];
                if (A[g][s] * B[s][v]  ==  1 && last[num][g][v].first== -1)
                {
                    last[num][g][v].first = s;
                    last[num][g][v].second.first = numA;
                    last[num][g][v].second.second = numB;
                }
            }
            (kek == 0) ? C[g][v] = 0 : C[g][v] = 1;
        }
    }
    return C;
}
std::vector<std::vector<int> > sum(std::vector<std::vector<int> >& A, std::vector<std::vector<int> >& B)
{
    std::vector<std::vector<int> > C(A.size(), std::vector<int>(B.size(), 0));
    for (int i = 0; i < A.size(); i++)
        for (int j = 0; j < B.size(); j++)
            (A[i][j] + B[i][j] == 0) ? C[i][j] = 0 : C[i][j] = 1;
    return C;
}
std::vector<std::vector<int> > razn(std::vector<std::vector<int> >& A, std::vector<std::vector<int> >& B) //difference
{
    std::vector<std::vector<int> > C(A.size(), std::vector<int>(B.size(), 0));
    for (int i = 0; i < A.size(); i++)
        for (int j = 0; j < B.size(); j++)
            (A[i][j] - B[i][j] <= 0) ? C[i][j] = 0: C[i][j] = 1;
    return C;
}

int check(std::vector<std::string>& a, std::string b, int& initial)
{
    for (int i = 0; i < a.size(); i++)
    {
        if (b == "S" && a[i] == b)
            initial = i;
        if (a[i] == b)
            return i;
    }
    if (b == "S")
        initial = a.size();
    return a.size();
}

void dfs(int i, std::vector<std::set<int> >& G, std::vector<int>& visited, int component, std::vector<int>& topsort)
{
    visited[i] = component;
    for (auto j: G[i])
        if (visited[j] == 0)
            dfs(j, G, visited, component, topsort);
    topsort.push_back(i);
}



std::vector<int> path_find(std::vector<std::vector<std::vector<std::string> > >& g_l, int begin, int end, int nont,     std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > >& last)
{
    if (last[nont][begin][end].first == -2)
    {
        std::vector<int> u;
        u.push_back(begin), u.push_back(end);
        return u;
    }
    int v = last[nont][begin][end].first, A = last[nont][begin][end].second.first, B = last[nont][begin][end].second.second;
    std::vector<int> left, right, res;
    left = path_find(g_l, begin, v, A, last), right = path_find(g_l, v, end, B, last);
    for (auto n: left)
        res.push_back(n);
    res.pop_back();
    for (auto n: right)
        res.push_back(n);
    return res;
}

void print_results(int test, int ans, std::vector<std::vector<std::vector<int> > >& g, std::vector <std::string>& V_names, std::vector<std::vector<std::vector<std::string> > >& g_l, int& initial, std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > >& last)
{
    std::cout << std::endl << "Graph N" << test << std::endl;
    std::cout << "Number of pairs = " << ans << std::endl;
    for (int i = 0; i < g[initial].size(); i++)
    {
        for (int j = 0; j < g[initial].size(); j++)
        {
            if (g[initial][i][j] == 1)
            {
                std::cout << "Path between " << V_names[i] << " " << V_names[j] << " with length ";
                std::vector<int> way = path_find(g_l, i, j, initial, last);
                std::cout << way.size() << std::endl;
                for (int r = 0; r < way.size(); r++)
                    std::cout << V_names[way[r]] << " ";
                std::cout << std::endl << std::endl;
            }
        }
    }
}

void find_rtdg(std::vector<std::vector<int> >& RTDG, std::vector<std::string>& nonterminals, std::vector<int>& topsort, std::vector<rule>& rules)
{
    std::vector<std::set<int> > G_nont(nonterminals.size()); //graph of nonterminals
    std::vector<std::set<int> > G_T(nonterminals.size()); //(graph of nonterminals)
    for (int i  = 0; i < rules.size(); i++)
    {
        if (rules[i].type == 1)
        {
            G_nont[rules[i].left].insert(rules[i].right1[0]);
            G_nont[rules[i].left].insert(rules[i].right1[1]);


            G_T[rules[i].right1[0]].insert(rules[i].left);
            G_T[rules[i].right1[1]].insert(rules[i].left);
        }
    }
    std::vector<int> visited(nonterminals.size(), 0);
    int component = 1;
    for (int i = 0; i < nonterminals.size(); i++)
        if (visited[i] == 0)
            dfs(i, G_nont, visited, 1, topsort);
    std::vector<int> visited1(nonterminals.size(), 0);
    for (int i = topsort.size() - 1; i >= 0; i--)
    {
        if (visited1[topsort[i]] == 0)
        {
            dfs(topsort[i], G_T, visited1, component, topsort);
            std::vector<int> l;
            l.push_back(topsort[i]);
            RTDG.push_back(l);
            component += 1;
        }
        else
            RTDG[visited1[topsort[i]] - 1].push_back(topsort[i]);
    }
}

void transitive_closure(int& flag, std::vector<std::vector<std::vector<int> > >& delta,std::vector<std::vector<int> >& RTDG, std::vector<std::vector<std::vector<int> > >& g, int i, int k, std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > >& last, std::vector<rule>& rules)
{
    flag = 1;
    std::vector<std::vector<int> > delta_I, temp;
    delta_I = delta[i];
    delta[i] = razn(delta[i], delta[i]);
    for (auto h: rules)
    {
        if (h.type == 1 && h.right1[0] == i) {
            temp = mult(delta_I, g[h.right1[1]], h.left, i, h.right1[1], last);
            temp = razn(temp, g[h.left]);
            delta[h.left] = sum(delta[h.left], temp);
            g[h.left] = sum(g[h.left], delta[h.left]);
        }
        if (h.type == 1 && h.right1[1] == i) {
            temp = mult(g[h.right1[0]], delta_I, h.left, h.right1[0], i, last);
            temp = razn(temp, g[h.left]);
            delta[h.left] = sum(delta[h.left], temp);
            g[h.left] = sum(g[h.left], delta[h.left]);
        }
    }
}

void input_rules(int i, std::vector<std::string>& nonterminals, std::vector<rule>& rules, std::ifstream& fin, int& initial)
{
    std::string left, right, a_2 = "", a_3 = "";
    fin >> left >> right;
    //! ================ strange code might be refactored ================ !
    int num = right.size();
    if (num != 1 && num != 2) {
        std::cout << "right.size() != (1 || 2)" << std::endl;
        exit(-1);
    }
    std::vector<int> FLAG(3,0);
    (num == 2) ? (a_2 = right[0], a_3 = right[1]) : a_2 = right;
    std::vector<std::string> a = {left, a_2, a_3};
    for (int i = 0; i < ((num == 1) ? 1 : 3); i++) {
        FLAG[i] = check(nonterminals, a[i], initial);
        if (FLAG[i] == nonterminals.size())
            nonterminals.push_back(a[i]);
    }
    rules.push_back({num - 1, FLAG[0], {FLAG[1], FLAG[2]}, a_2});
    //! ================                                  ================ !
}

void arranging_rules_to_edges(int i, int& eps, std::vector<std::pair<int, std::pair<int, std::string> > >& edges, std::vector<std::vector<std::vector<int> > >& g, std::vector<std::vector<std::vector<std::string> > >& g_l, int& initial, std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > >& last, std::vector<rule>& rules, int V)
{
    std::vector<std::string> A;
    std::vector<std::vector<int> > G_A(V, std::vector<int>(V, 0));
    std::vector<std::vector<std::string> > G_L(V, std::vector<std::string>(V));
    for (auto j : rules)
        if (j.type == 0 && j.left == i)
            (j.right0 == "0") ? eps = 1, A.push_back(j.right0) : A.push_back(j.right0);
    for (auto j : edges)
        for (auto u : A)
            if (j.second.second == u)
                G_A[j.first][j.second.first] = 1, G_L[j.first][j.second.first] = u, last[i][j.first][j.second.first] = {-2, {-2, -2}};
    if (eps == 1)
        for (int j = 0; j < V; j++)
            G_A[j][j] = 1, G_L[j][j] = "0", last[i][j][j] = {-2, {-2, -2}};
    g.push_back(G_A), g_l.push_back(G_L);
}

void input_V_names(std::vector <std::string>& V_names, std::vector<std::pair<int, std::pair<int, std::string> > >& edges, std::ifstream& fin, int V, int E)
{
    for (int i = 0; i < V; i++)
        fin >> V_names[i];
    for (int i = 0, u1, u2; i < E; i++)	{
        std::string a;
        fin >> u1 >> u2 >> a;
        edges.push_back({u1, {u2, a}});
    }
}

int count_ans(std::vector<std::vector<std::vector<int> > >& g, int initial)
{
    int ans = 0;
    for (auto i : g[initial])
        for (auto j : i)
            if (j == 1)
                ans++;
    return ans;
}