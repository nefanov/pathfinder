// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar,
// next line path
#include <iostream>
#include <vector>
#include <set>
#include <filesystem>
#include <fstream>
using namespace std;


struct rule
{
    int type;
    int left;
    vector<int> right1;
    string right0;
};


vector<rule> rules;
vector<int> topsort;
vector<vector<vector<pair<int, pair<int, int> > > > > last;
int initial; //S - nonterminal


int not_null(vector<vector<int> > A) //function for checking if A is zero
{
    int flag = 0;
    for (int i = 0; i < A.size(); i++)
    {
        for (int j = 0; j < A.size(); j++)
        {
            if (A[i][j] == 1)
            {
                return 1;
            }
        }
    }
    return 0;
}


vector<vector<int> > mult(vector<vector<int> > A,  vector<vector<int> > B, int num, int numA, int numB) //multiplication
{
    vector<vector<int> > C(A.size());
    for (int g = 0; g < A.size(); g++)
    {
        for (int v = 0; v < B.size(); v++)
        {
            int kek = 0;
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
            if (kek == 0)
            {
                C[g].push_back(0);
            }
            else
            {
                C[g].push_back(1);
            }
        }
    }
    return C;
}
vector<vector<int> > sum(vector<vector<int> > A, vector<vector<int> > B)
{
    vector<vector<int> > C(A.size());
    for (int g = 0; g < A.size(); g++)
    {
        for (int v = 0; v < B.size(); v++)
        {
            if (A[g][v] + B[g][v] == 0)
            {
                C[g].push_back(0);
            }
            else
            {
                C[g].push_back(1);
            }
        }
    }
    return C;
}
vector<vector<int> > razn(vector<vector<int> > A, vector<vector<int> > B) //difference
{
    vector<vector<int> > C(A.size());
    for (int g = 0; g < A.size(); g++)
    {
        for (int v = 0; v < B.size(); v++)
        {
            if (A[g][v] - B[g][v] <= 0)
            {
                C[g].push_back(0);
            }
            else
            {
                C[g].push_back(1);
            }
        }
    }
    return C;
}
int check(vector<string>& a, string b)
{

    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == b)
        {
            if (b == "S")
            {
                initial = i;
            }
            return i;
        }
    }
    if (b == "S")
    {
        initial = a.size();
    }
    return a.size();
}

void dfs(int i, vector<set<int> >& G, vector<int>& visited, int component)
{
    visited[i] = component;
    for (auto j: G[i])
    {
        if (visited[j] == 0)
        {
            dfs(j, G, visited, component);
        }
    }
    topsort.push_back(i);
}

vector<int> path_find(vector<vector<vector<string> > >& g_l, int begin, int end, int nont)
{
    if (last[nont][begin][end].first == -2)
    {
        vector<int> u;
        u.push_back(begin);
        u.push_back(end);

        return u;
    }
    int v = last[nont][begin][end].first;
    int A = last[nont][begin][end].second.first;
    int B = last[nont][begin][end].second.second;

    vector<int> left;
    left = path_find(g_l, begin, v, A);
    vector<int> right;
    right = path_find(g_l, v, end, B);
    vector<int> res;
    for (auto n: left)
    {

        res.push_back(n);
    }
    res.pop_back();
    for (auto n: right)
    {

        res.push_back(n);
    }
    return res;
}

int main()
{
    ifstream fin;
    fin.open("../data/graph");
    int tests;
    fin >> tests;
    for (int test = 0; test < tests; test++)
    {
	rules.clear();
	topsort.clear();
	last.clear();
	 //grammar
    	vector<string> nonterminals;
    	int m; //quantity of rules
    	fin >> m;
    	for (int i = 0; i < m; i++)
    	{
            string left, right;

            fin >> left >> right;

            int num;
            if (right.size() == 2)
            {
            	num = 2;
            }
            else if (right.size() == 1)
            {
            	num = 1;
            }

            if (num == 1)
            {
            	string a_1, a_2;
            	a_1 = left;
            	a_2 = right;

            	int FLAG  = check(nonterminals, a_1);





            	if (FLAG == nonterminals.size())
            	{
                    nonterminals.push_back(a_1);
            	}

            	rule a;

            	a.left = FLAG;
            	a.right0 = a_2;
            	a.type = 0;
            	rules.push_back(a);
            }
            else if (num == 2)
            {
            	string a_1, a_2, a_3;
            	a_1 = left;
            	a_2 = right[0];
            	a_3 = right[1];

            	int FLAG1  =  check(nonterminals, a_1);


            	if (FLAG1 == nonterminals.size())
            	{
                    nonterminals.push_back(a_1);
            	}


            	int FLAG2  =  check(nonterminals, a_2);

            	if (FLAG2 == nonterminals.size())
            	{
                    nonterminals.push_back(a_2);
            	}

            	int FLAG3  =  check(nonterminals, a_3);


            	if (FLAG3 == nonterminals.size())
            	{
                    nonterminals.push_back(a_3);
                }



                rule a;
                vector<int> q;
                q.push_back(FLAG2);
                q.push_back(FLAG3);
                a.left = FLAG1;
                a.right1 = q;
                a.type = 1;
                rules.push_back(a);
            }
    	}
    	int V, E;
    	fin >> V >> E;
    	vector<pair<int, pair<int, string> > > edges;
        vector <string> V_names(V);
        for (int i = 0; i < V; i++)
        {
            fin >> V_names[i];
        }
        for (int i = 0; i < E; i++)	
	{
            int u1, u2;
            string a;
            fin >> u1 >> u2 >> a;
            pair<int, pair<int, string> > v1;
            v1.first = u1;
            v1.second.first = u2;
            v1.second.second = a;
            edges.push_back(v1);
        }
    	pair<int, pair<int, int>> y;
    	y.first = -1;
    	y.second.first = -1;
    	y.second.second = -1;
	vector<vector<pair<int, pair<int, int> > > > l (V, vector<pair<int, pair<int, int> > > (V, y));
   	for (int i = 0; i < nonterminals.size(); i++)
    	{
            last.push_back(l);
    	}


   	 // Finding RTDG(rules)

    	vector<set<int> > G_nont(nonterminals.size()); //graph of nonterminals
    	vector<set<int> > G_T(nonterminals.size()); //(graph of nonterminals)^T
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
    	vector<int> visited(nonterminals.size(), 0);
    	int component = 1;
    	for (int i = 0; i < nonterminals.size(); i++)
    	{
            if (visited[i] == 0)
            {
            	dfs(i, G_nont, visited, 1);
            }
    	}
    	vector<vector<int> > RTDG;
    	vector<int> visited1(nonterminals.size(), 0);
        for (int i = topsort.size() - 1; i >= 0; i--)
        {
            if (visited1[topsort[i]] == 0)
            {
            	dfs(topsort[i], G_T, visited1, component);
            	vector<int> l;
            	l.push_back(topsort[i]);
            	RTDG.push_back(l);
            	component += 1;
            }
            else
            {
            	RTDG[visited1[topsort[i]] - 1].push_back(topsort[i]);
            }
    	}
    // Semi-Naive CFL
    	vector<vector<vector<int> > > g;
    	vector<vector<vector<string> > > g_l;

    	for (int i = 0; i < nonterminals.size(); i++)
    	{

            vector<string> A;
            int eps = 0;
            vector<vector<int> > G_A(V, vector<int>(V, 0));
            vector<vector<string> > G_L(V, vector<string>(V));
            for (int j = 0; j < rules.size(); j++)
            {

            	if (rules[j].type == 0 && rules[j].left == i)
            	{

                    A.push_back(rules[j].right0);
                    if (rules[j].right0 == "0")
                    {
                    	eps = 1;
                    }

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
                        pair<int, pair<int, int> > u;
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
                    pair<int, pair<int, int> > u;
                    u.first = -2;
                    u.second.first = -2;
                    u.second.second = -2;
                    last[i][j][j] = u;
            	}
            }
            g.push_back(G_A);
            g_l.push_back(G_L);

    	}
    	vector<vector<vector<int> > > delta;
    	delta = g;
    	for (int k = RTDG.size() - 1; k >= 0; k--)
    	{
            int flag = 1;
            while (flag)
            {
            	flag = 0;
            	for (int i = 0; i < RTDG[k].size(); i++)
            	{
                    if (not_null(delta[RTDG[k][i]]))
                    {
                    	flag = 1;
                    	vector<vector<int> > delta_I;
                    	delta_I = delta[RTDG[k][i]];
                    	delta[RTDG[k][i]] = razn(delta[RTDG[k][i]], delta[RTDG[k][i]]);
                    	for (auto h: rules)
                    	{
                            if (h.type == 1 && (h.right1[0] == RTDG[k][i]))
                            {
                                delta[h.left] = sum(delta[h.left], razn(mult(delta_I, g[h.right1[1]], h.left, RTDG[k][i], h.right1[1]), g[h.left]));
                                g[h.left] = sum(g[h.left], delta[h.left]);
                            }
                            if (h.type == 1 && (h.right1[1] == RTDG[k][i]))
                            {
                            	delta[h.left] = sum(delta[h.left], razn(mult(g[h.right1[0]], delta_I, h.left, h.right1[0], RTDG[k][i]), g[h.left]));
                            	g[h.left] = sum(g[h.left], delta[h.left]);
                            }

                    	}
                    }
            	}
            }
        }
	int ans = 0;
	for (int i = 0; i < g[initial].size(); i++)
	{
		for (int j = 0; j < g[initial].size(); j++)
		{
			if (g[initial][i][j] == 1)
			{
				ans++;
			}
		}
	}
	cout << "Graph N" << test << endl;
	cout << "Number of pairs = " << ans << endl;
     	for (int i = 0; i < g[initial].size(); i++)
    	{
            for (int j = 0; j < g[initial].size(); j++)
            {
            	if (g[initial][i][j] == 1)
                {
                    cout <<"Path between " << V_names[i] << " " << V_names[j] << " with length ";
                    vector<int> way;
                    way = path_find(g_l, i, j, initial);
		    cout << way.size() << endl;
                    for (int r = 0; r < way.size(); r++)
                    {
                        cout << V_names[way[r]] << " ";
                    }
                    cout << endl << endl;

                }
            }
    	}
	cout << endl;
    }
    fin.close();
}
