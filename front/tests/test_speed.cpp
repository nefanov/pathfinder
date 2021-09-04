#include <iostream>
#include <fstream>
#include <string>
int main()
{
    int type = 0, n = 0;
    std::ifstream test_list("list_graphs.txt");
    std::ifstream graph;
    while(!test_list.eof()) {
        test_list >> type >> n;
        system(("python testgraph_generator.py " + std::to_string(type) + " " + std::to_string(n)).c_str());
        // remember start clock
        system("../../build/core testgraph_generator.output -fast");
        // remember end clock
        // write start-end clock
        // remember start clock
        system("../../build/core testgraph_generator.output -slow");
        // remember end clock
        // write start-end clock
    }
    test_list.close();
}