#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <math.h>
#include <deque>
#include <fstream>
#include <dlfcn.h>
#include <string.h>
#define NUMBER_OF_LETTERS_WITH_OVERFLOW 30
class rule
{
    public:
        int type;
        int left;
        std::vector<int> right1;
        std::string right0;
};

class funcs
{
    public:
        void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P);
        std::vector<unsigned int> (*difference)(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2);
        std::vector<int> (*not_null)(int P, std::vector<unsigned int> kek);
        std::vector<int> (*create_wv)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2);
        std::unordered_set<int> (*create_wu)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2);
        std::vector<int> (*create_q)(int P, std::vector<unsigned int> v);
        std::vector <std::vector <std::vector <unsigned int> > > (*create_Hv)(int ntsize, int V, int P);
        std::vector <std::vector <std::unordered_set<int> > > (*create_Hu)(int ntsize, int V, int P);
        int (*create_P)(int V);

        funcs(void* sl)
        {
            add_value  = reinterpret_cast<void(*)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P)>(dlsym(sl, "add_value"));
            difference = reinterpret_cast<std::vector<unsigned int>(*)(int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2)>(dlsym(sl, "difference"));
            not_null   = reinterpret_cast<std::vector<int>(*)(int P, std::vector<unsigned int> kek)>(dlsym(sl, "not_null"));
            create_wv  = reinterpret_cast<std::vector<int>(*)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)>(dlsym(sl, "create_wv"));
            create_wu  = reinterpret_cast<std::unordered_set<int>(*)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2)>(dlsym(sl, "create_wu"));
            create_q   = reinterpret_cast<std::vector<int>(*)(int P, std::vector<unsigned int> v)>(dlsym(sl, "create_q"));
            create_Hv   = reinterpret_cast<std::vector <std::vector <std::vector <unsigned int> > >(*)(int ntsize, int V, int P)>(dlsym(sl, "create_Hv"));
            create_Hu   = reinterpret_cast<std::vector <std::vector <std::unordered_set <int> > >(*)(int ntsize, int V, int P)>(dlsym(sl, "create_Hu"));
            create_P   = reinterpret_cast<int(*)(int V)>(dlsym(sl, "create_P"));
        }
};


int check                              (int& initial, std::vector<std::string>& a, std::string b);

int input_rules                        (int& initial, std::vector <rule>& eps_rules, std::vector <std::vector<int>>& lol, 
                                        std::ifstream& fin, std::vector <std::string>& nonterminals, std::vector <rule>& rules);
int filling_edge_matrices(int P, std::ifstream& fin, std::vector <std::vector<int>>& lol, std::deque <std::vector<int>>& W, std::vector <std::vector <std::vector <unsigned int> > >& H1v, std::vector <std::vector <std::vector <unsigned int> > >& H2v, std::vector <std::vector <std::unordered_set <int> > >& H1u, std::vector <std::vector <std::unordered_set <int> > >& H2u, void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P));

int filling_loops(int j, int P, rule& i, std::deque <std::vector<int>>& W, std::vector <std::vector <std::vector <unsigned int> > >& H1v, std::vector <std::vector <std::vector <unsigned int> > >& H2v, std::vector <std::vector <std::unordered_set <int> > >& H1u, std::vector <std::vector <std::unordered_set <int> > >& H2u, void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P));



int baseline_cfl(bool is_fast, int flag, int i2, int i3, std::vector <std::vector <std::vector <unsigned int> > >& Hiv, std::vector <std::vector <std::unordered_set <int> > >& Hiu, std::vector<std::vector<int>> side_rules, int B, int P, int V, std::vector <rule>& rules, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::deque <std::vector<int>>& W, std::vector <std::vector <std::vector <unsigned int> > >& H1v, std::vector <std::vector <std::vector <unsigned int> > >& H2v, std::vector <std::vector <std::unordered_set <int> > >& H1u, std::vector <std::vector <std::unordered_set <int> > >& H2u, void (*add_value)(std::unordered_set<int>& u, std::vector<unsigned int>& v, int a, int P), std::vector<int> (*create_wv)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2), std::unordered_set<int> (*create_wu)(int P, int V, std::vector<unsigned int>& v1, std::vector<unsigned int>& v2, std::unordered_set<int>& u1, std::unordered_set<int>& u2));


std::vector<int> path_find             (int i, int j, int nonterm, std::vector<std::vector<std::vector<std::vector<int> > > >& prev);

int output                             (bool is_fast, int P, int V, int initial, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H1v, std::vector<std::vector<std::unordered_set<int>>>& H1u,
                                        std::vector<int>(*create_q)(int P, std::vector<unsigned int> v));


int find_arg(int argc, char* argv[], const char* arg);
funcs create_funcs(void* sl);
int check_funcs(funcs& func);