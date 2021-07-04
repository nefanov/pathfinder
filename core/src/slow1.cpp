using namespace std;
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <deque>
struct rule
{
    int type;
    int left;
    vector<int> right1;
    string right0;
};

int initial;
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

vector<int> path_find(int i, int j, int nonterm,  vector<vector<vector<vector<int> > > >& prev)
{
    vector<int> u = prev[i][nonterm][j];
    if (u[0] == -1 )
    {
        vector<int> kek;
        kek.push_back(i);
        kek.push_back(j);
        return kek;
    }
    vector<int> left = path_find(i, u[0], u[1], prev);
    vector<int> right = path_find(u[0], j, u[2],  prev);
    vector<int> way = left;
    way.pop_back();
    for (auto k: right)
    {
        way.push_back(k);
    }
    return way;
}

vector<rule> rules;

int main()
{
    //grammar
    vector<string> nonterminals;
    int m; //quantity of rules
    cin >> m;
    vector<rule> eps_rules;
    vector<vector<int> > lol(30);
    for (int i = 0; i < m; i++)
    {
        string left, right;

        cin >> left >> right;

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
            if (a_2 != "0")
            {
                lol[a_2[0] - 96].push_back(FLAG);
            }
            else
            {
                lol[0].push_back(FLAG);
                eps_rules.push_back(a);
            }
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
    vector<vector<int>> left_rules(nonterminals.size());
    vector<vector<int>> right_rules(nonterminals.size());
    for (int i = 0; i < rules.size(); i++)
    {
        if (rules[i].type == 1)
        {
            int u = rules[i].right1[0];
            int v = rules[i].right1[1];
            left_rules[u].push_back(i);
            right_rules[v].push_back(i);
        }
    }


    int V, E;
    cin >> V >> E;
    vector<pair<int, pair<int, string> > > edges;
    deque<vector<int> > W;
    vector<vector<unordered_set<int> > > H1 (nonterminals.size(), vector<unordered_set<int> > (V));
    vector<vector<unordered_set<int> > > H2 (nonterminals.size(), vector<unordered_set<int> > (V));
    vector<vector<vector<vector<int>> > > prev(V, vector<vector<vector<int > > > (nonterminals.size(), vector<vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
    {

        int u1, u2;
        string a;
        cin >> u1 >> u2 >> a;
        int s = 0;
        if (a != "0")
        {
            s = a[0] - 96;
        }

        for (auto j: lol[s])
        {
            vector<int> z;
            z.push_back(u1);
            z.push_back(j);
            z.push_back(u2);
            W.push_back(z);
            H1[j][u1].insert(u2);
            H2[j][u2].insert(u1);
        }

    }


    for (auto i: eps_rules)
    {
        for (int j = 0; j < V; j++)
        {
            vector<int> z;
            z.push_back(i.left);
            z.push_back(j);
            z.push_back(i.left);
            W.push_back(z);
            H1[j][i.left].insert(i.left);
            H2[j][i.left].insert(i.left);
        }
    }
    while (!W.empty())
    {
        vector<int> q;


        q = W.front();

        W.pop_front();

        int B = q[1];
        int u = q[0];
        int v = q[2];

        for (auto i:right_rules[B])
        {

            int C = rules[i].right1[0];
            int A = rules[i].left;
            unordered_set<int> w = H2[C][u];
            for (auto d: H2[A][v])
            {
                w.erase(d);
            }
            for (auto d: w)
            {
                H2[A][v].insert(d);
                H1[A][d].insert(v);
                vector<int> z;
                z.push_back(d);
                z.push_back(A);
                z.push_back(v);
                W.push_back(z);
                if (prev[d][A][v][0] == -1)
                {
                    prev[d][A][v][0] = u;
                    prev[d][A][v][1] = C;
                    prev[d][A][v][2] = B;
                }
            }
        }

        for (auto i:left_rules[B])
        {

            int C = rules[i].right1[1];
            int A = rules[i].left;
            unordered_set<int> w = H1[C][v];
            for (auto d: H1[A][u])
            {
                w.erase(d);
            }
            for (auto d: w)
            {
                H2[A][d].insert(u);
                H1[A][u].insert(d);
                vector<int> z;
                z.push_back(u);
                z.push_back(A);
                z.push_back(d);
                W.push_back(z);
                if (prev[u][A][d][0] == -1)
                {
                    prev[u][A][d][0] = v;
                    prev[u][A][d][1] = B;
                    prev[u][A][d][2] = C;
                }
            }
        }
    }
    int counter = 0;
    for (int i = 0; i < V; i++)
    {
        for (auto j: H1[initial][i])
        {
            cout << i << " " << j << endl;
            vector<int> way = path_find(i, j, initial,  prev);
            for (auto k: way)
            {
                cout << k << " ";
            }
            cout << endl;
            counter += 1;
        }
    }
    cout << counter << endl;







}

