# example of usage

# common imports
import os
import sys
import pydot

# API paths
current_path = os.path.dirname(os.path.abspath("."))
sys.path.append(os.path.join(current_path, "../front/preprocessor"))
sys.path.append(os.path.join(current_path, "../front/preprocessor/lexer"))

# API imports
import lexer
import extra_py_utils
from predicates import *

# specificators imports
from mem_spec import *

def prepare_graph_example():
    """
    creating graph with empty (any) edges labeling
    pattern_composer -- function to compose the pattern
    specializer -- function to adjust graph by specific edges, nodes, labels, to check some predicates etc
    """
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i 1.c -s m.dot -p pic25.png')
    wdir = os.path.join(current_path, "../front")
    in_graph = os.path.join(wdir, 'm.dot')
    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, 'processed_pic.png')
    os.chdir(current_path)
    print(in_graph, out_f, out_pic, current_path, os.getcwd())
    lexer.prepare_graph(in_graph, out_f, out_pic,
                        need_graph_save=True,
                        need_plot=True,
                        pattern_composer=None,
                        specializer=None)



def prepare_graph_from_article():
    #print("TODO: move the code here")
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i 1.c -s m.dot -p pic25.png')
    wdir = os.path.join(current_path, "../front")
    in_graph = os.path.join(wdir, 'm.dot')
    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, 'processed_pic.png')
    os.chdir(current_path)
    print(in_graph, out_f, out_pic, current_path, os.getcwd())
    lexer.prepare_graph(in_graph, out_f, out_pic,
                        need_graph_save=True,
                        need_plot=True,
                        pattern_composer=compose_mem_pattern,
                        specializer=specialize_Dflow)


def prepare_interproc_graph():
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i 1.c -s m.dot -p pic25.png')
    wdir = os.path.join(current_path, "../front")
    in_graph = os.path.join(wdir, 'm.dot')
    out_f = os.path.join(wdir, 'm.dot')
    out_pic = os.path.join(wdir, 'processed_pic.png')
    os.chdir(current_path)

    g = pydot.graph_from_dot_file(out_f)
    ft  = extra_py_utils.func_table("../front/1.c")
    augm_g = extra_py_utils.prepare_interproc_graph_var_trans(g[0], ft)
    print([n.get_name() for n in augm_g.get_nodes()])

    augm_g.write_png("interpr_1.c.png")


if __name__ == '__main__':
    if (len(sys.argv)<2 or (sys.argv[1] =="--test" and sys.argv[2]=="empty_labeling")):
        prepare_graph_example()
    elif (sys.argv[1]=="--test" and sys.argv[2]=="malloc_memset"):
       prepare_graph_from_article()

    elif (sys.argv[1]=="--test" and sys.argv[2]=="interproc"):
       prepare_interproc_graph()
