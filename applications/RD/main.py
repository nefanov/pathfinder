# example of usage

# common imports
import os
import sys
import pydot

# API paths
current_path = os.path.dirname(os.path.abspath("."))
sys.path.append(os.path.join(current_path, "../front/preprocessor"))
sys.path.append(os.path.join(current_path, "../front/preprocessor/lexer"))
sys.path.append(os.path.join(current_path, "../third-party"))

# API imports
import lexer
import extra_py_utils
import cycle_detector
from predicates import *

# specificators imports
from mem_spec import *

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

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
    return lexer.prepare_graph(in_graph, out_f, out_pic,
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
    return lexer.prepare_graph(in_graph, out_f, out_pic,
                        need_graph_save=True,
                        need_plot=True,
                        pattern_composer=compose_mem_pattern,
                        specializer=specialize_Dflow)


def prepare_custom_markup(scenario=None):
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i 1.c -s m.dot -p pic25.png --ssa')
    wdir = os.path.join(current_path, "../front")
    in_graph = os.path.join(wdir, 'm.dot')
    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, 'p_cycle_exit_markup.png')
    os.chdir(current_path)
    return lexer.prepare_graph(in_graph, out_f, out_pic, need_graph_save=True,need_plot=True,
                            pattern_composer=compose_pattern,
                            specializer=specialize_Dflow,
                            scenario=scenario
                )


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
    return augm_g


if __name__ == '__main__':
    if (len(sys.argv)<2 or (sys.argv[1] =="--test" and sys.argv[2]=="empty_labeling")):
        graph, mapping = prepare_graph_example()
    elif (sys.argv[1]=="--test" and sys.argv[2]=="malloc_memset"):
        graph, mapping = prepare_graph_from_article()

    elif (sys.argv[1]=="--test" and sys.argv[2]=="cycle_exit"):
        scenario = {
                    'type':'flowlists',
                    'data':{'yes_df_list': [],
                            'no_df_list' : [],
                            'yes_cf_list': [["any if_cond", "if_cond any"]], #
                            'no_cf_list' : [["return_val exit"]],
                            'rel_kinds'  : set()}
                    }
        graph, mapping = prepare_custom_markup(scenario)

    

    elif (sys.argv[1]=="--test" and sys.argv[2]=="interproc"):
       prepare_interproc_graph()

    elif (sys.argv[1]=="--test" and sys.argv[2] == "interproc_ba"):
        def read_c(fn="../../front/1.c"):
            with open(fn) as file:
                lines = file.readlines()
                return lines

        def highlight(code, colors={2:bcolors.OKCYAN,3:bcolors.OKCYAN,4:bcolors.FAIL,5:bcolors.OKCYAN,6:bcolors.OKGREEN,7:bcolors.OKCYAN, 22:bcolors.OKBLUE}):
            for idx, line in enumerate(code):
                if idx in colors:
                    line=colors[idx]+line+bcolors.ENDC
                print(line)

       
        code = read_c()
        highlight(code)

    elif (sys.argv[1]=="--test" and sys.argv[2]=="cycle_detector"):
    # print_labels -- verbose print of labels -- may be useless 
       color_palette =['blue','orange','green','lime','red','purple']
       G = prepare_interproc_graph()
       print("Test 1")
       print(os.getcwd())
       cycle_detector.detect(fn="../front/m.dot", only_shortest=True, print_labels=True)
       print("Test 2")
       cycle_detector.detect(fn="../front/m.dot", only_shortest=False, print_labels=True)
       print("Test 3")
       ret = cycle_detector.detect(fn="../front/m.dot", only_shortest=True, print_labels=False)
       print(ret)
       for idx, r in enumerate(ret):
           color = color_palette[idx % len(color_palette)]
           G = extra_py_utils.highlight_node_seq(G, r, color)

       G.write_png("../front/det_cycles3.png")
       print("Test 4")
       
       ret = cycle_detector.detect(fn="../front/m.dot", only_shortest=False, print_labels=False)
       print(ret)
       for idx, r in enumerate(ret):
           color = color_palette[idx % len(color_palette)]
           G = extra_py_utils.highlight_node_seq(G, r, color)

       G.write_png("../front/det_cycles4.png")

       

        
