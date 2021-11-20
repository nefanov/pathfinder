# example of usage

# common imports
import os
import sys
from networkx.generators.social import les_miserables_graph
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


def specialize_prev_mem_dep(graph, nodes, node_lex_dict, P):
    def get_accepted_types(nodes, ptrn):
        ret_=[]
        for k, N in nodes.items():
            if k.startswith(ptrn[:-1]):
                ret_.extend([(_n,{'type':k}) for _n in N])
        return ret_

    def collect_nodes(p, nodes):
        if (p.left['type'].endswith('*')):
            l_nodes_agg = get_accepted_types(nodes, p.left['type'])
        else:
            l_nodes_agg = [(_n,{'type':p.left['type']}) for _n in nodes[p.left['type']]]
        if (p.right['type'].endswith('*')):
            r_nodes_agg = get_accepted_types(nodes, p.right['type'])
        else:
            r_nodes_agg = [(_n,{'type':p.right['type']}) for _n in nodes[p.right['type']]]

        return l_nodes_agg, r_nodes_agg

    def spec_edge_label_emit(p, src_fmt, dst_fmt):
        label=""
        if "labeling" in p.extra.keys():
            for token in p.extra['labeling'].split():
                if token== "$label":
                    label += p.label
                elif token.startswith("$"):
                    tk = token.split(":")
                    if tk[0][1:]== "src":
                        label +=" " + src_fmt[tk[1]]
                    elif tk[0][1:]=="dst":
                        label +=" " + dst_fmt[tk[1]]
                else:
                    label += token
        else:
            label=p.label

        return label

    for idx, p in enumerate(P):
        l_nodes_agg, r_nodes_agg = collect_nodes(p, nodes)
        for _n in l_nodes_agg:
            n = _n[0]
            #lbl_l = "node", graph.get_node(n)[0].get_attributes()['label']
            n1s = node_lex_dict[n]['content']
            for _n2 in r_nodes_agg:
                n2 = _n2[0]
                #lbl_r = "node2", graph.get_node(n2)[0].get_attributes()['label']
                n2s = node_lex_dict[n2]['content']
                if shortest_path_check(graph, n, n2):
                    if p.predicate(n1s['format'], n2s['format'], {'src_type':_n[1]['type'], 'dst_type':_n2[1]['type']}):
                        #print("Predicate is true, ", lbl_l, lbl_r)
                        graph.add_edge(pydot.Edge(n,
                                    n2,
                                    color=p.params["edge_style"]["color"],
                                    style='dashed',
                                    label=spec_edge_label_emit(p, n1s['format'], n2s['format'])))

    return graph


def prepare_custom_markup(scenario=None, spec=specialize_prev_mem_dep, need_interproc_pass=True):
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i 1.c -s m.dot -p pic25.png')
    wdir = os.path.join(current_path, "../front")
    in_graph = os.path.join(wdir, 'm.dot')
    if need_interproc_pass:
        g = pydot.graph_from_dot_file(in_graph)
        ft  = extra_py_utils.func_table("../front/1.c")
        g[0] = extra_py_utils.prepare_interproc_graph_var_trans(g[0], ft)
        g[0].write_raw(in_graph)

    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, 'p_cycle_exit_markup.png')
    os.chdir(current_path)
    return lexer.prepare_graph(in_graph, out_f, out_pic, need_graph_save=True,need_plot=True,
                            pattern_composer=compose_pattern,
                            specializer=specialize_prev_mem_dep,
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

# == predicate ==
def check_ref_dep_mem(self, l, r, type):
    def depends_from(inst, dependency):
        if inst.find(dependency) != -1:
            return True
        return False

    if type['dst_type'] == "assign_aryphmetic_op":
        return l['left'] in [r['r_operand1'], r['r_operand2']]
    elif type['dst_type'] == "assign_function_call":
        return False # now it is intraproc

    return depends_from(r['right'], l['left'])


    return G

if __name__ == '__main__':
    if (len(sys.argv)<2 or (sys.argv[1] =="--test" and sys.argv[2]=="empty_labeling")):
        prepare_graph_example()
    elif (sys.argv[1]=="--test" and sys.argv[2]=="malloc_memset"):
       prepare_graph_from_article()

    elif (sys.argv[1]=="--test" and sys.argv[2]=="cycle_basic"):
        scenario = {
                    'type':'flowlists',
                    'data':{'yes_df_list': [],
                            'no_df_list' : 
                            [
                                            [ # relation
                                                lexer.glex.Relation(
                                                left={'type': "assign*"},right={'type': "assign*"}, # src and dst nodes of relation edge
                                                predicate=check_ref_dep_mem, # function-specializer
                                                extra={"disable_labeling":
                                                            "$label $src:left" # extra labeling fmt string
                                                },
                                                label="DF_dep_from", # default label
                                                params={"edge_style": {"color": "#f76d23"}})# parameters for edge visualizing
                                            ] 
                            ],
                            'yes_cf_list': [["any if_cond", "if_cond any"]], #
                            'no_cf_list' : [["return_val exit"]],
                            'rel_kinds'  : set()}
                    }
        graph, mapping = prepare_custom_markup(scenario)

    elif (sys.argv[1]=="--test" and sys.argv[2]=="mem_anti"):
        scenario = {
                    'type':'flowlists',
                    'data':{'yes_df_list': [],
                            'no_df_list' : 
                            [
                                            [ # relation
                                                lexer.glex.Relation(
                                                left={'type': "assign*"},right={'type': "assign*"}, # src and dst nodes of relation edge
                                                predicate=check_ref_dep_mem, # function-specializer
                                                extra={"disable_labeling":
                                                            "$label $src:left" # extra labeling fmt string
                                                },
                                                label="DF_dep_from", # default label
                                                params={"edge_style": {"color": "#f76d23"}})# parameters for edge visualizing
                                            ] 
                            ],
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

       

        
