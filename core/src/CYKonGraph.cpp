// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar,
// next line path
#include "header.h"

struct rule
{
    int type;
    int left;
    std::vector<int> right1;
    std::string right0;
};


std::vector<rule> rules;
std::vector<int> topsort;
std::vector<std::vector<std::vector<std::pair<int, std::pair<int, int> > > > > last;
int initial; //S - nonterminal


int not_null(std::vector<std::vector<int> >& A) //function for checking if A is zero
{
    for (auto i : A)
        for (auto j : i)
            if (j == 1)
                return 1;
    return 0;
}

std::vector<std::vector<int> > mult(std::vector<std::vector<int> >& A,  std::vector<std::vector<int> >& B, int num, int numA, int numB) //multiplication
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

int check(std::vector<std::string>& a, std::string b)
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

void dfs(int i, std::vector<std::set<int> >& G, std::vector<int>& visited, int component)
{
    visited[i] = component;
    for (auto j: G[i])
        if (visited[j] == 0)
            dfs(j, G, visited, component);
    topsort.push_back(i);
}

std::vector<int> path_find(std::vector<std::vector<std::vector<std::string> > >& g_l, int begin, int end, int nont)
{
    if (last[nont][begin][end].first == -2)
    {
        std::vector<int> u;
        u.push_back(begin), u.push_back(end);
        return u;
    }
    int v = last[nont][begin][end].first, A = last[nont][begin][end].second.first, B = last[nont][begin][end].second.second;
    std::vector<int> left, right, res;
    left = path_find(g_l, begin, v, A), right = path_find(g_l, v, end, B);
    for (auto n: left)
        res.push_back(n);
    res.pop_back();
    for (auto n: right)
        res.push_back(n);
    return res;
}

int main()
{
    std::ifstream fin;
    fin.open("../data/graph");
    int tests;
    fin >> tests;
    for (int test = 0; test < tests; test++)
    {
        rules.clear();
        topsort.clear();
        last.clear();
        //grammar
        std::vector<std::string> nonterminals;
        int m; //quantity of rules
        fin >> m;
        for (int i = 0, num; i < m; i++)
        {
            std::string left, right;

            fin >> left >> right;
            if (right.size() == 1 || right.size() == 2)
                num = right.size();

            if (num == 1)
            {
                std::string a_1 = left, a_2 = right;
                int FLAG  = check(nonterminals, a_1);
                if (FLAG == nonterminals.size())
                    nonterminals.push_back(a_1);
                rule a;
                a.left = FLAG, a.right0 = a_2, a.type = 0;
                rules.push_back(a);
            }
            else if (num == 2)
            {
                std::string a_1 = left, a_2, a_3;
                a_2 = right[0];
                a_3 = right[1];
                int FLAG1 = check(nonterminals, a_1), FLAG2  = check(nonterminals, a_2), FLAG3  =  check(nonterminals, a_3);
                if (FLAG1 == nonterminals.size())
                    nonterminals.push_back(a_1);
                if (FLAG2 == nonterminals.size())
                    nonterminals.push_back(a_2);
                if (FLAG3 == nonterminals.size())
                    nonterminals.push_back(a_3);

                rule a;
                std::vector<int> q;
                q.push_back(FLAG2);
                q.push_back(FLAG3);
                a.left = FLAG1, a.right1 = q, a.type = 1;
                rules.push_back(a);
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

        std::vector<std::set<int> > G_nont(nonterminals.size()); //graph of nonterminals
        std::vector<std::set<int> > G_T(nonterminals.size()); //(graph of nonterminals)^T
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
                dfs(i, G_nont, visited, 1);
        std::vector<std::vector<int> > RTDG;
        std::vector<int> visited1(nonterminals.size(), 0);
        for (int i = topsort.size() - 1; i >= 0; i--)
        {
            if (visited1[topsort[i]] == 0)
            {
                dfs(topsort[i], G_T, visited1, component);
                std::vector<int> l;
                l.push_back(topsort[i]);
                RTDG.push_back(l);
                component += 1;
            }
            else
                RTDG[visited1[topsort[i]] - 1].push_back(topsort[i]);
        }
    // Semi-Naive CFL
        std::vector<std::vector<std::vector<int> > > g;
        std::vector<std::vector<std::vector<std::string> > > g_l;

        for (int i = 0, eps = 0; i < nonterminals.size(); i++, eps = 0)
        {
            std::vector<std::string> A;
            std::vector<std::vector<int> > G_A(V, std::vector<int>(V, 0));
            std::vector<std::vector<std::string> > G_L(V, std::vector<std::string>(V));
            for (int j = 0; j < rules.size(); j++)
            {
                if (rules[j].type == 0 && rules[j].left == i)
                {
                    A.push_back(rules[j].right0);
                    if (rules[j].right0 == "0")
                        eps = 1;
                }

            }
            for (int j = 0; j < edges.size(); j++)
            {
                for (auto u : A)
                {
                    if (edges[j].second.second == u)
                    {
                        G_A[edges[j].first][edges[j].second.first] = 1;
                        G_L[edges[j].first][edges[j].second.first] = u;
                        std::pair<int, std::pair<int, int> > u;
                        u.first = -2;
                        u.second.first = -2;
                        u.second.second = -2;
                        last[i][edges[j].first][edges[j].second.first] = u;

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
                for (int i = 0; i < RTDG[k].size(); i++)
                {
                    if (not_null(delta[RTDG[k][i]]))
                    {
                        flag = 1;
                        std::vector<std::vector<int> > delta_I;
                        delta_I = delta[RTDG[k][i]];
                        delta[RTDG[k][i]] = razn(delta[RTDG[k][i]], delta[RTDG[k][i]]);
                        for (auto h: rules)
                        {
                            if (h.type == 1 && (h.right1[0] == RTDG[k][i] || h.right1[1] == RTDG[k][i])) {
                                std::vector<std::vector<int>> temp;
                                temp = (h.right1[0] == RTDG[k][i]) ? mult(delta_I, g[h.right1[1]], h.left, RTDG[k][i], h.right1[1]) : mult(g[h.right1[0]], delta_I, h.left, h.right1[0], RTDG[k][i]);
                                delta[h.left] = sum(delta[h.left], temp);
                                g[h.left] = sum(g[h.left], delta[h.left]);
                            }
                        }
                    }
                }
            }
        }
        int ans = 0;
        for (int i = 0; i < g[initial].size(); i++)
            for (int j = 0; j < g[initial].size(); j++)
                if (g[initial][i][j] == 1)
                    ans++;
        std::cout << "Graph N" << test << std::endl;
        std::cout << "Number of pairs = " << ans << std::endl;
        for (int i = 0; i < g[initial].size(); i++)
        {
            for (int j = 0; j < g[initial].size(); j++)
            {
                if (g[initial][i][j] == 1)
                {
                    std::cout <<"Path between " << V_names[i] << " " << V_names[j] << " with length ";
                    std::vector<int> way;
                    way = path_find(g_l, i, j, initial);
                    std::cout << way.size() << std::endl;
                    for (int r = 0; r < way.size(); r++)
                        std::cout << V_names[way[r]] << " ";
                    std::cout << std::endl << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }
    fin.close();
}
