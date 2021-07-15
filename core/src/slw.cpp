#include "core.h"


std::unordered_set<int> create_w(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)
{
    std::unordered_set<int> w = u1;
    for (auto d: v2)
        w.erase(d);  
    return w;
}

void add_value(std::unordered_set<unsigned int>& u, std::vector<unsigned int>& v, int a, int P)
{
    u.insert(a);
}

std::vector<unsigned int> create_q(int P, std::vector<unsigned int> v)
{
    return v;
}

int create_P(int V)
{
    return 0;
}

std::vector<std::vector<std::unordered_set<int>>> create_Hu(int ntsize, int V, int P)
{
    std::vector<std::vector<std::unordered_set<int>>> H (ntsize, std::vector<std::unordered_set<int>> (V));
    return H;
}

std::vector<std::vector<std::vector<int>>> create_Hv(int ntsize, int V, int P)
{
    std::vector<std::vector<std::vector<int>>> H;
    return H;
}