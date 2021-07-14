#include "core.h"
void add_value                         (std::vector<unsigned int> &kek, int a, int P);

std::vector<unsigned int> difference   (int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2);

std::vector<int> not_null              (int P, std::vector<unsigned int> kek);

std::vector<int> create_w(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2);
std::vector<int> create_q(int P, std::vector<int> v);
std::vector <std::vector <std::vector <unsigned int> > > create_H(int ntsize, int V, int P);
int create_P(int V);