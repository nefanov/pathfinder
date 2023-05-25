#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
int main()
{
    int type = 0, n = 0;
    std::ifstream test_list("tests/speed_tests/list_graphs.txt");
    std::ofstream output("tests/speed_tests/test_speed.output");
    while(!test_list.eof()) {
        test_list >> type >> n;
        system(("python tests/speed_tests/graph_generator.py " + std::to_string(type) + " " + std::to_string(n)).c_str());
        auto start1 = std::chrono::high_resolution_clock::now();   // remember start clock
        //system("(build/core tests/speed_tests/graph_generator -fast) 2>> save.out");
        auto end1 = std::chrono::high_resolution_clock::now();     // remember end clock
        output << std::endl << std::endl;

        auto start2 = std::chrono::high_resolution_clock::now();   // remember start clock
        //system("(build/core tests/speed_tests/graph_generator -slow) 2>> save.out");
        auto end2 = std::chrono::high_resolution_clock::now();    // remember end clock
        output << std::endl << std::endl;
        
        auto start3 = std::chrono::high_resolution_clock::now();   // remember start clock
        system("(build/core tests/speed_tests/graph_generator -spaced_rhs -iguana) > save.out");
        auto end3 = std::chrono::high_resolution_clock::now();     // remember end clock
        output << std::endl << std::endl;
        
	output << type << " " << n << " " << std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count() << " " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << " " << std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3).count() << std::endl;
    }
    test_list.close();
}