# common imports
import os
import sys

# API paths
current_path = os.path.dirname(os.path.abspath("."))
sys.path.append(os.path.join(current_path, "../front/preprocessor/lexer"))

# API imports
import lexer

if __name__ == '__main__':
    print(lexer.glex.parse_function_def_args("int a, const char *b"))
    pass




