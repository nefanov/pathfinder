#include "orchestrator.h"

options::options(int argc, char* argv[])
{
    std::string s;
    for (int i = 1; i < argc; i++) {
        s = argv[i];
        if (s == "-cmd")
            is_using_cliin = true, path_to_analyzed_file = argv[++i], input_type = 1;
        if (s == "-file")
            is_using_filein = true, path_to_input_file = argv[++i], input_type = 2;
        if (s == "-front_only")
            is_front_only = true;
        if (s == "-thin")
            is_thin = true;
        if (s == "-fast")
            is_fast = true;
        if (s == "-slow")
            is_slow = true;
        if (s == "-o")
            is_saving_output = true, path_to_save_file = argv[++i];
	if (s == "-SSA")
	    ir_file_suffix = IR_SUFFIX_SSA;
    }
}
