#include <string>
#include <vector>

#define IR_SUFFIX_DFL ".012t.cfg.dot"
#define IR_SUFFIX_SSA ".019t.ssa.dot"

class options
{
    public:
        bool is_using_stdin = false;
        bool is_using_cliin = false;
        bool is_using_filein = false;
        bool is_front_only = false;
        bool is_thin = false;
        bool is_fast = false;
        bool is_slow = false;
        bool is_saving_output = false;
        std::string path_to_input_file = "";
	std::string ir_file_suffix = IR_SUFFIX_DFL;
        std::string path_to_analyzed_file = "";
        std::string path_to_save_file = "";
        int input_type = 0;
        options();
        options(int argc, char* argv[]);
};
