#include "code2graph.h"

int process_path(int argc, std::string path, std::ifstream& input_file)
{
    if (argc < 2) {
		std::cout << "Enter the name of the file to be analyzed, pre-compiled with gcc file_name -fdump-tree-cfg-graph: ";
		std::cin >> path;
	}
	path += ".012t.cfg.dot";
	input_file.open(path);
	if (!input_file.is_open()) {
		std::cout << "file was not opened" << std::endl;
		return -1;
	}
    return 0;
}