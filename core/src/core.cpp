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
#include <vector>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {
    bool is_iguana = (find_arg(argc, argv, "-iguana") > 0) ? true : false;
    bool is_time = (find_arg(argc, argv, "-time") > 0) ? true : false;
    int infd[2], outfd[2], pid;
    char buf;
    if (is_iguana) {
    	char* pwd = new char[PATH_MAX];
    	char conf[6][5];
    	int numgr, numv, numr;
    	ifstream file(argv[1]);
    	file.is_open();
    	file >> numgr;
    	file >> numgr;
    	string s, str[numgr];
    	FILE* grammar = fopen("../third-party/GLL4Graph/data/grammar.json", "w");
    	fprintf(grammar, "%s", "{\n\t\"kind\" : \"Grammar\",\n\t\"rules\" : [\n");
    	getline(file, s);
    	std::vector<string> words;
    	std::vector<string> nonterm;
    	words.push_back("\n");
    	for (int i = 0; i < numgr; i++) {
    		getline(file, s);
    		std::istringstream ist(s);
    		int sch = 0;
    		for (std::string st; getline(ist, st, ' '); words.push_back(st)) {
    			if (sch == 0) nonterm.push_back(st);
    			sch += 1;
    		}
    		words.push_back("\n");
    	}
    	int count = -2;
    	for (int i = 0; i < words.size() - 1; i++) {
    		if ((words[i] == "\n") && (count != -2)) count = -1;
    		if (count < 0) {
    			if (count == -1) fprintf(grammar, "%s", "\n\t\t\t]\n\t\t},\n");
    			fprintf(grammar, "%s", 
    			"\t\t{\n\t\t\t\"kind\" : \"Rule\",\n\t\t\t\"head\" : {");
    			count = 0;
    		} else if (count == 0) {
    			if (words[i] == "0") fprintf(grammar, "%s", "\n\t\t\t},\n\t\t\t\"body\" : [");
    			else 
    			fprintf(grammar, "%s%s%s%s", "\n\t\t\t\t\"kind\" : \"Nonterminal\",", 
    			"\n\t\t\t\t\"name\" : \"", words[i].c_str(), "\"\n\t\t\t},\n\t\t\t\"body\" : [");
    			count = 1;
    		} else if (count > 0) {
    			if (count != 1) fprintf(grammar, "%s", ",");
    			int ch = 0;
    			count += 1;
    			for (auto& el : nonterm) {
    				if (words[i] == el) ch = 1;
    			}
    			if (words[i] != "0") {
	    			if (ch != 1) {
	    				fprintf(grammar, "%s%s%s%s", "\n\t\t\t\t{\n\t\t\t\t\t\"kind\" : \"Terminal\",",
	    				"\n\t\t\t\t\t\"name\" : \"", words[i].c_str(), "\",");
	    				fprintf(grammar, "%s%s", "\n\t\t\t\t\t\"regex\" : {\n\t\t\t\t\t\t\"kind\":", 						"\"Seq\",\n\t\t\t\t\t\t\"symbols\": [\n");
	    				for (int j = 0; j < strlen(words[i].c_str()) - 1; j++) fprintf(grammar, "%s%d%s",
	    				 "{\t\t\t\t\t\t\t\"kind\": \"Char\", \"val\": ", (words[i].c_str())[j], "},");
	    				fprintf(grammar, "%s%d%s", 
	    				"\t\t\t\t\t\t\t{\"kind\": \"Char\", \"val\": ", 
	    				(words[i].c_str())[strlen(words[i].c_str()) - 1], "}");
	    				fprintf(grammar, "%s", "\n\t\t\t\t\t\t]\n\t\t\t\t\t}\n\t\t\t\t}");
	    			}
	    			else
	    			fprintf(grammar, "%s%s%s%s", "\n\t\t\t\t{\n\t\t\t\t\t\"kind\" : \"Nonterminal\",",
	    			 "\n\t\t\t\t\t\"name\" : \"", words[i].c_str(), "\"\n\t\t\t\t}");
    			}
    		}
    	}
    	fprintf(grammar, "%s", "\n\t\t\t]\n\t\t}\n");
    	fprintf(grammar, "%s%s", "\t],\n\t\"startSymbol\" : {\n\t\t\"kind\" : \"Start\",\n\t\t\"name\" : \"S\",", 
    	"\n\t\t\"nonterminal\" : {\n\t\t\t\"kind\" : \"Nonterminal\",\n\t\t\t\"name\" : \"S\"\n\t\t}\n\t}\n}");
    	fclose(grammar);
    	file >> numv >> numr;
    	string from, to, type;
    	getline(file, s);
    	getline(file, s);
	FILE* edges = fopen("../third-party/GLL4Graph/data/edges.csv", "a");
	fprintf(edges, "%s\n", "name:START_ID :TYPE name:END_ID");
    	for (int i = 0; i < numr; i++) {
    		file >> from >> to;
    		file.get();
    		getline(file, type);
    		if (type != "") fprintf(edges, "%s %s %s\n", from.c_str(), type.c_str(), to.c_str());
    		else fprintf(edges, "%s %s %s\n", from.c_str(), "other", to.c_str());
    	}
    	fclose(edges);
    	FILE* nodes = fopen("../third-party/GLL4Graph/data/nodes.csv", "w");
    	fprintf(nodes, "%s\n", "name:ID");
    	for (int i = 0; i < numv; i++) {
    		fprintf(nodes, "%d\n", i);
    	}
    	fclose(nodes);
    	char* numver = new char[numv];
    	sprintf(numver, "%d", numv);
    	file.close();
    	ifstream config("../core/config/config");
    	config.is_open();
    	for (int i = 0; i < 2; i++) {
    		config.getline(conf[i], 5, '\n');
    	}
    	config.close();
    	pipe(infd);
    	pipe(outfd);
    	pid = fork();
    	if (pid == 0) {
    		close(outfd[0]);
    		chdir("../third-party/GLL4Graph/");
    		getwd(pwd);
    		dup2(outfd[1], 1);
    		string syst;
    		syst = "mvn exec:java -Dexec.mainClass=\"benchmark.GraphBenchmark\" -Dexec.args=\"";
    		syst += "-d graph -gm ";
    		syst += pwd;
    		syst += "/data/grammar.json -gp ";
    		syst += pwd;
    		syst += "/data/ -gs IN_MEMORY -m ";
    		syst += conf[1];
    		syst += " -p ALL_PATHS -S s=ALL_PAIRS -S a=";
    		syst += numver;
    		syst += " -w ";
    		syst += conf[0];
    		syst += "\"";
    		FILE* r = fopen("AllPaths.txt", "a");
    		fclose(r);
    		setenv("JAVA_HOME", "/usr/lib/jvm/jdk-15", true);
      		system(syst.c_str());
    		string path = pwd;
    		path += "/results/graph_AP_SPPF_INMEM.csv";
    		if (is_time) {
    			int rt = 0;
    			ifstream rs(path);
    			rs.is_open();
    			while (getline(rs, s))  {
    				if (rt != 0) std::cout << "№" << rt << ": ";
    				rt += 1;
    				std::cout << s << endl;
    			}
    		}
    		ifstream results(path);
    		results.is_open();
    		getline(results, s);
    		getline(results, s);
    		string numb;
    		int counter = 0;
    		for (int k = 0; k < s.size(); k++) {
    			char nn = s[k];
    			if (nn == ',') {
    				counter += 1;
    			} else if (counter == 2) {
    				numb += nn;
    			}
    		}
    		std::cout << numb << endl;
    		results.close();
    		path = pwd;
    		path += "/AllPaths.txt";
    		ifstream res(path);
    		res.is_open();
    		while (getline(res, s)) { 
    			std::cout << s << endl;
    		}
    		res.close();
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
    	remove("../third-party/GLL4Graph/data/nodes.csv");
    	remove("../third-party/GLL4Graph/data/edges.csv");
	remove("../third-party/GLL4Graph/data/grammar.json");
	remove("../third-party/GLL4Graph/results/graph_AP_SPPF_INMEM.csv");
	remove("../third-party/GLL4Graph/AllPaths.txt");
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
    unsigned int start_time =  clock();
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
    unsigned int end_time = clock();
    fin.close();
    if (is_time) {
    	std::cout << "run_time = " << (end_time - start_time)/1000.0 << " sec" << endl;
    }
    return 0;
}
