#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unordered_set>
using namespace std;

int main(int argc, char* argv[]) {
	std::random_device prng;
	std::string s0, s, st, s1, s2, ss;
	int numv, numr, n = 0;
	map <string, int> mp;
	vector <string> tags;
	tags = {"a", "b", ""};
	vector <string> nodes;
	s0 = argv[1];
	s = s0.substr(s.rfind(" ") + 1);
	ifstream file (s.c_str());
	s = s0.substr(s0.rfind(" ") + 1, s0.rfind("/") - s0.rfind(" "));
	ss = s;
	s += "graph";
	s += s0.substr(s0.find(".main") + 5, s0.rfind(".") - s0.find(".main") - 5);
	ofstream f (s.c_str());
	numr = 0;
	numv = 0;
	srand(time(NULL));
	while (getline(file, st)) {
		if (st.find("->") != -1) numr += 1;
		if (st.find("Node") != -1) {
			s1 = st.substr(st.find("Node"), st.find_first_of(" :") - st.find("Node") - 1);
			nodes.push_back(s1);
			if (mp.find(s1) == mp.end()) {	
				mp.insert(make_pair(s1, n));
				n += 1;
			}
		}
		std::sort(nodes.begin(), nodes.end());
    		nodes.erase(std::unique(nodes.begin(), nodes.end()), nodes.end());
	}
	s = s0.substr(s.rfind("/") + 1);
	file.clear();
	file.seekg(0);
	numv = nodes.size();
	f << "1" << endl;
	ifstream f1((ss + "grammatics.txt").c_str());
	while (getline(f1, st)) {
		f << st << endl;
	}
	f1.close();
	f << endl;
	f << numv << " " << numr << endl;
	for (int i = 0; i < nodes.size(); i++) f << nodes[i] << " ";
	f << endl;
	std::uniform_int_distribution<int> dist(1, nodes.size());
	while (getline(file, st)) {
		if (st.find("->") != -1) {
			s1 = st.substr(st.find("Node"), st.find_first_of(": ") - st.find("Node") - 1);
			s2 = st.substr(st.rfind("Node"), st.find_first_of(" ;[", st.rfind("Node")) - st.rfind("Node") - 1);
			f << mp[s1] << " " << mp[s2] << " " << tags[dist(prng) % 3] << endl;
		}
	}
	f.close();
	file.close();
	remove("transl_to_core");
}