using namespace std;
#define max(a, b) (a > b) ? a : b
#include <iostream>
#include <vector>
#include <deque>
struct rule
{
    int left;
    vector<int> right;
};

int main()
{
    vector<rule> rules, rules1, rules2, rules3, rules4, rules5;
    int N, let_quantity, a, len, t;

    cin >> let_quantity >> N;
    int counter = let_quantity + 3;
    for (int i = 0; i < N; i++)
    {
        cin >> a >> len;
        counter = max(a + 1, counter);
        rule b = {a};
        for (int j = 0; j < len; j++)
        {
            cin >> t;
            (b.right).push_back(t);
            counter = max(t + 1, counter);
        }
        rules.push_back(b);
    }


    //long rules deleting


    for (int i = 0; i < rules.size(); i++)
    {

        if ((rules[i].right).size() > 2)
        {
            counter += 1;
            rules1.push_back({rules[i].left, {(rules[i].right)[0], counter}});
            rules1.push_back({counter + 1, {(rules[i].right)[1], (rules[i].right)[2]}});
        }
        else
            rules1.push_back(rules[i]);
    }

    //epsilon-rules deleting


    vector<int> eps_net(counter, 0);
    vector<int> counter_rules(rules1.size());
    deque<int> deq, deq2;
    vector<vector<int> > g(counter, vector<int> (rules1.size(), 0));
    

    for (int k = 0; k < rules1.size(); k++)
    {
        for (auto j: rules1[k].right)
        {
            g[j][k] += 1;
            if (j > let_quantity + 1)
                counter_rules[k] += 1;
        }
    }

    for (int j = 0; j < rules1.size(); j++)
    {

        if ((rules1[j].right)[0] == 0)
        {
            deq.push_back(rules1[j].left);
            counter_rules[j] -= 1;
            eps_net[rules1[j].left] = 1;
        }
    }


    while (!deq.empty())
    {
        int c = deq.front();
        deq.pop_front();
        for (int i = 0; i < rules1.size(); i++)
        {
            if (g[c][i] != 0)
            {
                counter_rules[i] -= g[c][i];
                if (counter_rules[i] == 0 && eps_net[rules1[i].left] == 0)
                {
                    eps_net[rules1[i].left] = 1;
                    deq.push_back(rules1[i].left);
                }
            }
        }
    }

    for (int i = 0; i < rules1.size(); i++)
    {
        if ((rules1[i].right).size() == 2)
        {
            if (eps_net[rules1[i].right[0]] == 1)
                rules2.push_back({rules1[i].left, {(rules1[i].right)[1]}});
            if (eps_net[rules1[i].right[1]] == 1)
                rules2.push_back({rules1[i].left, {(rules1[i].right)[0]}});
            rules2.push_back(rules1[i]);
        }
        else
            if ((rules1[i].right)[0] != 0)
                rules2.push_back(rules1[i]);
    }
    if (eps_net[let_quantity + 2] == 1)
    {
        for (int i = 0; i < rules2.size(); i++)
        {
            if (rules2[i].left == let_quantity + 2)
                rules2[i].left = counter;
            if (rules2[i].right[0] == let_quantity + 2)
                rules2[i].right[0] = counter;
            if (rules2[i].right.size() > 1  && rules2[i].right[1] == let_quantity + 2)
                rules2[i].right[1] = counter;
        }
        rules2.push_back({let_quantity + 2, {{counter}}});
        rules2.push_back({let_quantity + 2, {{0}}});
        counter += 1;
    }


    //cout << "||||||||||" << endl;
    //for (auto j: rules1)
    //{
    //    cout << j.left << " -> " ;
    //    for (auto k: j.right)
     //   {
    //        cout << k << " ";
    //    }
    //    cout << endl;
    //}
    //cout << "||||||||||" << endl;
    //for (auto j: rules2)
    //{
    //    cout << j.left << " -> " ;
    //    for (auto k: j.right)
    //    {
    //        cout << k << " ";
    //    }
    //    cout << endl;
    //}


    //cepnie pravila

    deque<pair<int, int> > deq1;
    vector<vector<int> > g1(counter), G(counter);
    for (int i = 0; i < rules2.size(); i++)
    {
        G[rules2[i].left].push_back(i);
        if ((rules2[i].right).size() == 1 && rules2[i].right[0] > let_quantity)
            g1[rules2[i].left].push_back(rules2[i].right[0]);
    }
    vector<vector<int> > visited(counter, vector<int> (counter, 0));
    for (int i = let_quantity + 2; i < counter; i++)
    {
        deq1.push_back({i, i});
        visited[i][i] = 1;
    }
     while (!deq1.empty())
     {
         pair<int, int> c = deq1.front();
         deq1.pop_front();
         for (auto j: g1[c.second])
         {
             if (!visited[c.first][j])
             {
                 visited[c.first][j] = 1;
                 deq1.push_back({c.first, j});
             }
         }
     }

     for (int i = 0; i < counter; i++)
         for (int j = 0; j < counter; j++)
             if (i != j)
                 if (visited[i][j] == 1)
                     for (auto q: G[j])
                         if ((rules2[q].right).size() > 1 || rules2[q].right[0] < let_quantity + 1)
                             rules3.push_back({i, rules2[q].right});
     for (auto i: rules2)
     {
         if ((i.right).size() > 1)
             rules3.push_back(i);
         else if (i.right[0] < let_quantity + 1)
             rules3.push_back(i);
     }
    //cout << "||||||||||" << endl;
    //for (auto j: rules3)
    //{
    //    cout << j.left << " -> " ;
    //    for (auto k: j.right)
    //    {
    //        cout << k << " ";
    //    }
    //    cout << endl;
    //}


    //deleting useless symbols

    vector<int> reachable(counter, 0), counter_rules3(rules3.size(), 0);
    vector<vector<int> > g3(counter, vector<int> (rules3.size(), 0));

    for (int i = 0; i < rules3.size(); i++)
    {
        for (auto j: rules3[i].right)
        {
            g3[j][i] += 1;
            if (j > let_quantity + 1)
                counter_rules3[i] += 1;
        }
        if (counter_rules3[i] == 0)
        {
            deq2.push_back(rules3[i].left);
            reachable[rules3[i].left] = 1;
        }
    }

    while (!deq2.empty())
    {
        int a = deq2.front();
        deq2.pop_front();
        for (int i = 0; i < rules3.size(); i++)
        {
            if (g3[a][i] != 0)
                counter_rules3[i] -= g3[a][i];
            if (counter_rules3[i] == 0 && reachable[rules3[i].left == 0])
            {
                reachable[rules3[i].left] = 1;
                deq2.push_back(rules3[i].left);
            }
        }
    }

    for (auto i:rules3)
    {
        if (reachable[i.left])
        {
            int flag = 1;
            for (auto j: i.right)
                if (!reachable[j] && j > let_quantity + 1)
                    flag = 0;
            if (flag)
                rules4.push_back(i);
        }
    }


    //cout << "|||||||||| " << endl;
    //for (auto j: rules4)
    //{
    //    cout << j.left << " -> " ;
    //    for (auto k: j.right)
    //    {
    //        cout << k << " ";
    //    }
    //    cout << endl;
    //}


    //changing rules with two terminals
    for (auto i: rules4)
    {
        if ((i.right).size() > 1 && i.right[0] < let_quantity + 1 && i.right[1] < let_quantity + 1)
        {
            rules5.push_back({i.left, {{counter}, {counter + 1}}});
            rules5.push_back({counter, {i.right[0]}});
            rules5.push_back({counter + 1, {i.right[1]}});
            counter += 2;
        }
        else
            rules5.push_back(i);
    }
    cout << rules5.size() << endl;
    for (auto j: rules5)
    {
        cout << j.left << " " << (j.right).size();
        cout << endl;
        for (auto k: j.right)
            cout << k << " ";
        cout << endl;
    }
}