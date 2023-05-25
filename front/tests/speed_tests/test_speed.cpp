#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
int main()
{
    int type = 0, n = 0;
    clock_t start1, end1, start2, end2, start3, end3, start4, end4;
    std::ifstream test_list("tests/speed_tests/list_graphs.txt");
    std::ofstream output("tests/speed_tests/test_speed.output");
    //while(!test_list.eof()) {
        //test_list >> type >> n;
        n = 10;
        system(("python tests/speed_tests/graph_generator.py " + std::to_string(type) + " " + std::to_string(n)).c_str());
        start1 = clock();   // remember start clock
        //system("(build/core graph_generator.output -fast) 2>> save.out");
        end1 = clock();     // remember end clock
        output << std::endl << std::endl;

        start2 = clock();   // remember start clock
        //system("(build/core graph_generator.output -slow) 2>> save.out");
        end2 = clock();     // remember end clock
        output << std::endl << std::endl;
        
        start3 = clock();   // remember start clock
        system("(build/core graph_generator.output -spaced_rhs -iguana) 2>> save.out");
        end3 = clock();     // remember end clock
        output << std::endl << std::endl;
        
        output << type << " " << n << " " << static_cast<double>(end1 - start1) / CLOCKS_PER_SEC << " " << static_cast<double>(end2 - start2) / CLOCKS_PER_SEC << " " << static_cast<double>(end3 - start3) / CLOCKS_PER_SEC << std::endl;// write start-end clock
        output << start1 << " " << end1 << " " << start2 << " " << end2 << " " << start3 << " " << end3 << std::endl << std::endl;// write start-end clock
    //}
    test_list.close();
}
