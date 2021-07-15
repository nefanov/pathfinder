#include "core.h"


extern "C" {
std::vector<unsigned int> new_fastset(int P, int V)
{
    int k = (V % P == 0) ? (V / P): (V / P) + 1;
    std::vector<unsigned int> fset(k, 0);
    return fset;
}

void add_value(std::unordered_set<unsigned int>& u, std::vector<unsigned int>& v, int a, int P)
{
    v[a / P] |= (1 << (a % P));
}

std::vector<unsigned int> difference(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)
{
    std::vector<unsigned int> ans = new_fastset(P, V);
    for (int i = 0; i < kek1.size(); i++) {
        unsigned int s1 = kek2[i], s = ~s1;
        s &= ((1 << P) - 1);
        ans[i] = (kek1[i] & (s));
    }
    return ans;
}

std::vector<int> not_null(int P, std::vector<unsigned int> kek)
{
    std::vector<int> ans;
    for (int i = 0; i < kek.size(); i++) {
        if (kek[i] != 0) {
            unsigned int m = kek[i];
            for (int j = 0; j < P; j++, m /= 2)
                if (m % 2 == 1)
                    ans.push_back(j + i * P);
        }
    }
    return ans;
}

std::vector<int> create_wv(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)
{
    return not_null(P, difference(P, V, v1, v2));
}

std::unordered_set<int> create_wu(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)
{
    std::unordered_set<int> w = u1;
    return w;
}

std::vector<int> create_q(int P, std::vector<unsigned int> v)
{
    return not_null(P, v);
}

int create_P(int V)
{
    return (V != 1) ? ceil(log10(V)): 1; //defining the length of binary number
}

std::vector<std::vector<std::vector<unsigned int>>> create_Hv(int ntsize, int V, int P)
{
    std::vector <std::vector <std::vector <unsigned int>>> H(ntsize, std::vector<std::vector <unsigned int > > (V, new_fastset(P, V)));
    return H;
}


std::vector<std::vector<std::unordered_set<int>>> create_Hu(int ntsize, int V, int P)
{
    std::vector<std::vector<std::unordered_set<int>>> H (ntsize, std::vector<std::unordered_set<int>> (V));
    return H;
}
}