#include <iostream>
#include <vector>
#include <string>
using namespace std;
// input 1)quantity of rules (Homsky Grammar) 2)rules: "A BC" 3)quantity of vertexes, quantity of edges
// 4)edges: "1 2 u"
// output: pairs of vertexes (u, v): there is a path u->v, and the word, created by the path, contains in grammar
struct rule
{
    string left;
    vector<string> right;
};

int main()
{
    int m; //quantity of rules
    cin >> m;
    vector<rule> rules; //grammar
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
            rule a;
            vector<string> m;
            m.push_back(a_2);
            a.left = a_1;
            a.right = m;
            rules.push_back(a);
        }
        else if (num == 2)
        {
            string a_1, a_2, a_3;
            a_1 = left;
            a_2 = right[0];
            a_3 = right[1];
            rule a;
            vector<string> m;
            m.push_back(a_2);
            m.push_back(a_3);
            a.left = a_1;
            a.right = m;
            rules.push_back(a);
        }
    }

    int V, E;
    cin >> V >> E;
    vector<pair<int, pair<int, string> >  > edges;
    for (int i = 0; i < E; i++)
    {
        int u1, u2;
        string a;
        cin >> u1 >> u2 >> a;
        pair<int, pair<int, string> > v1;
        v1.first = u1;
        v1.second.first = u2;
        v1.second.second = a;
        edges.push_back(v1);

    }

    vector<vector<vector<string>>> M(V, vector<vector<string>> (V));
    int flag = 1;

    //base
    for (int i = 0; i < E; i++)
    {
        for (int j = 0; j < rules.size(); j++)
        {
            vector<string> let = rules[j].right;
            if (let.size() == 1 && let[0] == edges[i].second.second)
            {
                int flag2 = 0;
                for (int k = 0; k < M[edges[i].first][edges[i].second.first].size(); k++)
                {
                    if (M[edges[i].first][edges[i].second.first][k] == rules[j].left)
                    {
                        flag2 = 1;
                    }
                }
                if (flag2 == 0)
                {
                    M[edges[i].first][edges[i].second.first].push_back(rules[j].left);
                }
            }
        }
    }

    for (int i = 0; i < rules.size(); i++)
    {
        vector<string> t;
        t = rules[i].right;
        if (t.size() == 1 && t[0][0] == '0')
        {
            for (int j = 0; j < V; j++)
            {
                int flag2 = 0;
                for (int k = 0; k < M[j][j].size(); k++)
                {
                    if (M[j][j][k] == rules[i].left)
                    {
                        flag2 = 1;
                    }
                }
                if (flag2 == 0)
                {
                    M[j][j].push_back(rules[i].left);
                }
            }
        }
    }


    //algorithm
    while (flag)
    {
        flag = 0;
        for (int k = 0; k < V; k++)
        {
            for (int i = 0; i < V; i++)
            {
                for (int j = 0; j < V; j++)
                {
                    for (int g = 0; g < M[i][k].size(); g++)
                    {
                        for (int v = 0; v < M[k][j].size(); v++)
                        {
                            for (int f = 0; f < rules.size(); f++)
                            {
                                vector<string> t;
                                t = rules[f].right;
                                string r = rules[f].left;
                                if (t.size() > 1)
                                {
                                    if (t[0] == M[i][k][g] && t[1] == M[k][j][v])
                                    {
                                        int flag1 = 0;
                                        for (int y = 0; y < M[i][j].size(); y++)
                                        {
                                            if (M[i][j][y] == r)
                                            {
                                                flag1 = 1;
                                            }
                                        }
                                        if (flag1 == 0)
                                        {
                                            flag = 1;
                                            M[i][j].push_back(r);
                                        }
                                    }
                                }

                            }
                        }

                    }
                }
            }

        }
    }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            for (int k = 0; k < M[i][j].size(); k++)
            {
                if (M[i][j][k][0] == 'S')
                {
                    cout << i << " " << j << endl;
                    break;
                }
            }
        }
    }
}
