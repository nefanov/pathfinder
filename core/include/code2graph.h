#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

int process_path(int argc, std::string path, std::ifstream& input_file);
