using namespace std;
#include <iostream>
#include <vector>
#include <deque>
struct func
{
    int u;
    int v;
    int let;
    int stack_pop;
    vector<int> stack_push;
};
struct rule
{
    int left;
    vector<int> right;
};


int main()
{
    int V, E, let_quantity, initial;
    cin >> V >> E;
    cin >> initial; //initial state of PA
    cin >> let_quantity;
    int counter = let_quantity + 2;
    int counter1 = 0;
    vector<func> PM;


    for (int i = 0; i < E; i++)
    {
        int u, v, let, stack_pop, stack_push_len;
        cin >> u >> let >> stack_pop >> v >> stack_push_len;
        vector<int> stack_push;
        for (int j = 0; j < stack_push_len; j++)
        {
            int a;
            cin >> a;
            stack_push.push_back(a);
        }
        if (stack_push_len > 2)
        {
            PM.push_back({u, v, let, stack_pop, {{counter}, {stack_push[stack_push_len - 1]}}});
            counter += 1;
            for (int j = 1; j < stack_push_len - 2; j++)
            {
                PM.push_back({v, v, 0, counter - 1, {{counter}, {stack_push[stack_push_len - j - 1]}}});
                counter += 1;
                counter1 += 1;
            }
            PM.push_back({v, v, 0, counter - 1, {{stack_push[0]}, {stack_push[1]}}});
        }
        else
            PM.push_back({u, v, let, stack_pop, stack_push});
    }
    for (int j = 0; j < PM.size(); j++)
    {
        if ((PM[j].stack_push)[0] == let_quantity + 1)
            PM[j].stack_push[0] = counter - 1;
        else if ((PM[j].stack_push)[0] > let_quantity + 1)
            PM[j].stack_push[0] -= 1;


        if ((PM[j].stack_pop) == let_quantity + 1)
            PM[j].stack_pop = counter - 1;
        else if ((PM[j].stack_pop) > let_quantity + 1)
            PM[j].stack_pop -= 1;


        if ((PM[j].stack_push)[1] == let_quantity + 1)
            PM[j].stack_push[1] = counter - 1;
        else if ((PM[j].stack_push)[1] > let_quantity + 1)
            PM[j].stack_push[1] -= 1;
    }
    let_quantity = counter - 2;

    counter = let_quantity + 3;
    //S = let_quantity + 2
    //e = 0
    //z0 = let_quantity + 1
    vector<rule> rules;
    vector<vector<vector<int> > > nonterminals(V, vector<vector<int > > (let_quantity + 2, vector<int> (V, -1)));
    for (int i = 0; i < V; i++)
    {
        nonterminals[initial][let_quantity + 1][i] = counter;
        rules.push_back({let_quantity + 2, {counter}});
        counter += 1;
    }
    for (auto j: PM)
    {
        int u, v, stack_pop, let, stack_push;
        if ((j.stack_push).size() == 1)
        {
            u = j.u, v = j.v, let = j.let, stack_pop = j.stack_pop, stack_push = j.stack_push[0];
            if (stack_push == 0)
            {
                if (nonterminals[u][stack_pop][v] == -1)
                    nonterminals[u][stack_pop][v] = counter, counter += 1;
                rules.push_back({nonterminals[u][stack_pop][v], {let}});
            }
            else
            {
                for (int j = 0; j < V; j++)
                {
                    if (nonterminals[u][stack_pop][j] == -1)
                        nonterminals[u][stack_pop][j] = counter, counter += 1;
                    if (nonterminals[u][stack_push][j] == -1)
                        nonterminals[u][stack_push][j] = counter, counter += 1;
                    rule a;
                    a.left = nonterminals[u][stack_pop][j];
                    if (let != 0)
                        (a.right).push_back(let);
                    (a.right).push_back(nonterminals[u][stack_push][j]);
                    rules.push_back(a);
                }
            }
        }
        else
        {
            for (int r1 = 0; r1 < V; r1++)
            {
                for (int r2 = 0; r2 < V; r2++)
                {
                    u = j.u, v = j.v, let = j.let, stack_pop = j.stack_pop;
                    if (nonterminals[u][stack_pop][r2] == -1)
                        nonterminals[u][stack_pop][r2] = counter, counter += 1;
                    if (nonterminals[v][j.stack_push[0]][r1] == -1)
                        nonterminals[v][j.stack_push[0]][r1] = counter, counter += 1;
                    if (nonterminals[r1][j.stack_push[1]][r2] == -1)
                        nonterminals[r1][j.stack_push[1]][r2] = counter, counter += 1;
                    rule a;
                    a.left = nonterminals[u][stack_pop][r2];
                    if (let != 0)
                        (a.right).push_back(let);
                    (a.right).push_back(nonterminals[v][j.stack_push[0]][r1]);
                    (a.right).push_back(nonterminals[r1][j.stack_push[1]][r2]);
                    rules.push_back(a);
                }
            }
        }
    }

    cout << rules.size() << endl;
    for (auto kek: rules)
    {
        cout << kek.left << " " << (kek.right).size() << endl;
        for (auto j:kek.right)
            cout << j << " ";
        cout << endl;
    }
}