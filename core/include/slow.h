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
int check                  (std::vector<std::string>& a, std::string b, int& initial);

int input_rules            (int& initial, std::vector <rule>& eps_rules, std::vector <std::vector<int>>& lol, 
                            std::ifstream& fin, std::vector <std::string>& nonterminals, std::vector <rule>& rules);

int filling_edge_matrices  (std::ifstream& fin, std::vector <std::vector<int>>& lol, std::deque <std::vector<int>>& W, 
                            std::vector <std::vector <std::unordered_set <int> > >& H1, 
                            std::vector <std::vector <std::unordered_set <int> > >& H2);

int filling_loops          (int j, int P, rule& i, std::deque <std::vector<int>>& W, 
                            std::vector <std::vector <std::unordered_set <int> > >& H1, 
                            std::vector <std::vector <std::unordered_set <int> > >& H2);

int baseline_cfl           (int flag, int i2, int i3, std::vector <std::vector <std::unordered_set <int> > >& Hi, 
                            std::vector<std::vector<int>> side_rules, int B, std::vector <rule>& rules, 
                            std::vector<std::vector<std::vector<std::vector<int>> > >& prev, std::deque <std::vector<int>>& W, 
                            std::vector <std::vector <std::unordered_set <int> > >& H1, 
                            std::vector <std::vector <std::unordered_set <int> > >& H2);

std::vector<int> path_find (int i, int j, int nonterm,  std::vector<std::vector<std::vector<std::vector<int> > > >& prev);

int output                 (int V, int initial, std::vector<std::vector<std::vector<std::vector<int>> > >& prev, 
                            std::vector <std::vector <std::unordered_set <int> > >& H1);