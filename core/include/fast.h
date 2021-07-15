#include "core.h"


std::vector<unsigned int> new_fastset(int P, int V);
void add_value(std::unordered_set<unsigned int>& u, std::vector<unsigned int>& v, int a, int P);
std::vector<unsigned int> difference(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2);
std::vector<int> not_null(int P, std::vector<unsigned int> kek);
std::vector<int> create_w(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2);
std::vector<int> create_q(int P, std::vector<unsigned int> v);
int create_P(int V);
std::vector<std::vector<std::vector<unsigned int>>> create_H(int ntsize, int V, int P);
