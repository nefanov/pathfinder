#include <iostream>
#include <vector>
#include <set>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

class rule {
    public:
        int type;
        int left;
        std::vector<int> right1;
        std::string right0;
};