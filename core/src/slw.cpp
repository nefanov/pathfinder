#include "core.h"

std::unordered_set<int> create_w(std::unordered_set<int>& v1, std::unordered_set<int>& v2)
{
    std::unordered_set<int> w = v1;
    for (auto d: v2)
        w.erase(d);  
    return w;
}

void add_value(std::unordered_set<unsigned int> &u, std::vector<unsigned int> &v, unsigned int a, int P)
{
    u.insert(a);
}

std::vector<int> create_q(int P, std::vector<int> v)
{
    return v;
}

int create_P(int V)
{
    return 0;
}

std::vector<std::vector<std::unordered_set<int> > > create_H(int ntsize, int V, int P)
{
    std::vector<std::vector<std::unordered_set<int> > > H (ntsize, std::vector<std::unordered_set<int> > (V));
    return H;
}