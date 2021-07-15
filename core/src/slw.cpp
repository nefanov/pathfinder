#include "core.h"

extern "C" {
std::unordered_set<int> create_wu(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)
{
    std::unordered_set<int> w = u1;
    for (auto d: u2)
        w.erase(d);  
    return w;
}

std::vector<int> create_wv(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)
{
    std::vector<int> wv(V, 0);
    return wv;
}

void add_value(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P)
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

std::vector<std::vector<std::vector<unsigned int>>> create_Hv(int ntsize, int V, int P)
{
    std::vector <std::vector <std::vector <unsigned int>>> H(ntsize, std::vector<std::vector <unsigned int>> (V, std::vector<unsigned int>(V*2, 0)));
    return H;
}

std::vector<unsigned int> new_fastset(int P, int V)
{
    std::vector<unsigned int> fset;
    return fset;
}

std::vector<unsigned int> difference(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)
{
    std::vector<unsigned int> ans;
    return ans;
}

std::vector<int> not_null(int P, std::vector<unsigned int> kek)
{
    std::vector<int> ans;
    return ans;
}
}