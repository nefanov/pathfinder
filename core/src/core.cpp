#include "core.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <fstream> 
#include <limits.h>
using namespace std;

int main(int argc, char* argv[]) {
    bool is_iguana = (find_arg(argc, argv, "-iguana") > 0) ? true : false;
    int infd[2], outfd[2], pid;
    char buf;
    char* pwd = new char[PATH_MAX];
    string s;
    int numgr, numv, numr;
    /*//преобр теста
    ifstream file(argv[1]);
    file.is_open();
    file >> numgr;
    file >> numgr;
    std::cout << numgr;
    for (int i = 0; i < numgr; i++) {
    	file >> s;
    	std::cout << s << "\n";
    }
    file >> s;
    file >> numv >> numr;
    std::cout << numv << "*" << numr;
    //конец преобр теста*/
    if (is_iguana) {
    	pipe(infd);
    	pipe(outfd);
    	pid = fork();
    	if (pid == 0) {
    		close(outfd[0]);
    		chdir("../third-party/GLL4Graph/");
    		getwd(pwd);
    		dup2(outfd[1], 1);
    		char* syst = 
    		new char[strlen("mvn exec:java -Dexec.mainClass=\"benchmark.Neo4jBenchmark\" -Dexec.args=\"st 1323 2 5 ")
    		 + strlen("/data/core/ test/resources/grammars/graph/g1/grammar.json core g1\"") + 1 + strlen(pwd)];
    		strcat(syst, "mvn exec:java -Dexec.mainClass=\"benchmark.Neo4jBenchmark\" -Dexec.args=\"st 1323 2 5 ");
    		strcat(syst, pwd);
    		strcat(syst, "/data/core/ test/resources/grammars/graph/g1/grammar.json core g1\"");
    		system(syst);
    		close(outfd[1]);
    	}
    	else {
    		close(outfd[1]);
    		while (read(outfd[0], &buf, 1) > 0) {
    			write(1, &buf, 1);
    		}
    		close(outfd[0]);
    		wait(NULL);
    	}
	return 0;
    }
    bool is_fast = (find_arg(argc, argv, "-fast") > 0) ? true : false;
    bool is_spaced_rhs =(find_arg(argc, argv, "-spaced_rhs") > 0) ? true : false;
    void* sl = dlopen((is_fast == true) ? "libfst.so" : "libslw.so", RTLD_LAZY);
    std::cout << ((is_fast == true) ? "libfst.so" : "libslw.so") << std::endl;
    funcs func(sl);
    if (check_funcs(func) < 0)
        return -1;
    int m, number, E, initial, P, V;
    std::vector <std::string> nonterminals;
    std::unordered_map <std::string, std::vector<int>> terminal_symbol_table;
    std::vector <Rule> eps_rules, rules;
    std::string path_to_graph = argv[1];
    std::ifstream fin(path_to_graph);
    if (!fin.is_open()) {
        std::cout << path_to_graph << " was not opened" << std::endl;
        return -1;
    }
    
    fin >> number >> m;
    if (is_spaced_rhs) {
        // preventive read of input to the end of line
        std::string prev_read_str;
        std::getline(fin, prev_read_str);
    }
    for (int i = 0; i < m; i++)
        input_rule(initial, eps_rules, terminal_symbol_table, fin, nonterminals, rules, is_spaced_rhs);

    std::vector <std::vector<int>> left_rules(nonterminals.size()), right_rules(nonterminals.size());
    for (int i = 0; i < rules.size(); i++)
        if (rules[i].type == 1)
            left_rules[rules[i].right1[0]].push_back(i), right_rules[rules[i].right1[1]].push_back(i);
 
    fin >> V >> E; //input number of vertexes and edges
    std::vector<std::string> nodes_names(V);
    for (int i = 0; i < V; i++)
        fin >> nodes_names[i];

    P = func.create_P(V);
    std::vector<std::pair<int, std::pair<int, std::string> > > edges;
    std::deque<std::vector<int> > W;
    std::vector<std::vector<std::vector<unsigned int>>>H1v = func.create_Hv(nonterminals.size(), V, P), H2v = H1v;
    std::vector<std::vector<std::unordered_set<int>>>H1u = func.create_Hu(nonterminals.size(), V, P), H2u = H1u;
    std::vector<std::vector<std::vector<std::vector<int>> > > prev(V, std::vector<std::vector<std::vector<int > > > (nonterminals.size(), 
			    std::vector<std::vector<int>>  (V, {-1, -1, -1})));
    for (int i = 0; i < E; i++)
        filling_edge_matrices(P, fin, terminal_symbol_table, W, H1v, H2v, H1u, H2u, func.add_value);
    for (auto i: eps_rules)
        for (int j = 0; j < V; j++)
            filling_loops(j, P, i, W, H1v, H2v, H1u, H2u, func.add_value); //if word is empty
    while (!W.empty()) {
        std::vector<int> q = W.front();
        W.pop_front();
        baseline_cfl(is_fast, 0, q[0], q[2], H2v, H2u, right_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
        baseline_cfl(is_fast, 1, q[2], q[0], H1v, H1u, left_rules, q[1], P, V, rules, prev, W, H1v, H2v, H1u, H2u, func.add_value, func.create_wv, func.create_wu);
    }
    output(is_fast, P, V, initial, prev, H1v, H1u, func.create_q);
    fin.close();
    return 0;
}
