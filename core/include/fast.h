#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <math.h>
#include <deque>
#include <fstream>
#define NUMBER_OF_LETTERS_WITH_OVERFLOW 30
class rule
{
    public:
        int type;
        int left;
        std::vector<int> right1;
        std::string right0;
};

int check                              (int& initial, std::vector<std::string>& a, std::string b);

int input_rules                        (int& initial, std::vector <rule>& eps_rules, std::vector <std::vector<int>>& lol, 
                                        std::ifstream& fin, std::vector <std::string>& nonterminals, std::vector <rule>& rules);

std::vector<unsigned int> new_fastset  (int P, int V);

void add_value                         (std::vector<unsigned int> &kek, int a, int P);

int filling_edge_matrices              (int P, std::ifstream& fin, std::vector <std::vector<int>>& lol, std::deque <std::vector<int>>& W,
                                        std::vector <std::vector <std::vector <unsigned int> > >& H1, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H2);

int filling_loops                      (int j, int P, rule& i, std::deque <std::vector<int>>& W, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H1, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H2);

std::vector<unsigned int> difference   (int P, int V, std::vector<unsigned int> &kek1, std::vector<unsigned int> &kek2);

std::vector<int> not_null              (int P, std::vector<unsigned int> &kek);

int baseline_cfl                       (int flag, int i2, int i3, std::vector <std::vector <std::vector <unsigned int> > >& Hi, 
                                        std::vector<std::vector<int>> side_rules, int B, int P, int V, std::vector <rule>& rules, 
                                        std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::deque <std::vector<int>>& W, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H1, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H2);

std::vector<int> path_find             (int i, int j, int nonterm, std::vector<std::vector<std::vector<std::vector<int> > > >& prev);

int output                             (int P, int V, int initial, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, 
                                        std::vector <std::vector <std::vector <unsigned int> > >& H1);