# example of usage

# common imports
import os
import sys
import subprocess
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

    for p in P:
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
                    if p.predicate(n1s['format'], n2s['format'], {'src_type':_n[1]['type'], 'dst_type':_n2[1]['type']}) and n != n2:
                        #print("Predicate is true, ", lbl_l, lbl_r)
                        graph.add_edge(pydot.Edge(n,
                                    n2,
                                    color=p.params["edge_style"]["color"],
                                    style='dashed',
                                    label=spec_edge_label_emit(p, n1s['format'], n2s['format'])))

    return graph


def prepare_custom_markup(scenario=None, spec=specialize_prev_mem_dep, need_interproc_pass=True, config=None):
    if config:
        working_dir = config['working_dir']
        src = config['src']
        ir_dotfile = config['ir_dotfile']
        ir_pic = config['ir_pic']
        ir_markup = config['ir_markup']
    else:
        working_dir = os.path.join(current_path, "../front")
        src = "1.c"
        ir_dotfile = "m.dot"
        ir_pic = "pic25.png"
        ir_markup = 'p_markup.png'

    wdir = working_dir
    os.chdir("../../front")
    os.system('./get_thin_graph.sh -i ' + os.path.join(wdir, src) + ' -s '+ os.path.join(wdir, ir_dotfile) + ' -p '+os.path.join(wdir,ir_pic))
    in_graph = os.path.join(wdir, ir_dotfile)
    if need_interproc_pass:
        g = pydot.graph_from_dot_file(in_graph)
        ft  = extra_py_utils.func_table(os.path.join(wdir, src)) #"../front/1.c")
        g[0] = extra_py_utils.prepare_interproc_graph_var_trans(g[0], ft)
        g[0].write_raw(in_graph)

    out_f = os.path.join(wdir, 'processed.dot')
    out_pic = os.path.join(wdir, ir_markup)
    os.chdir(current_path)
    return lexer.prepare_graph(in_graph, out_f, out_pic, need_graph_save=True,need_plot=True,
                            pattern_composer=compose_pattern,
                            specializer=specialize_prev_mem_dep,
                            scenario=scenario
                ), working_dir


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

    if type['src_type'] == "assign_function_call" and l['left'] is None:
        return False

    if type['dst_type'] == "assign_aryphmetic_op":
        return l['left'] in [r['r_operand1'], r['r_operand2']]
    elif type['dst_type'] == "assign_function_call":
        return False   # now it is intraproc

    return depends_from(r['right'], l['left'])

# == predicate ==
def check_dup_func(self, l, r, type):
    if type['src_type'] == type['dst_type'] == "assign_function_call" and l['func_name'] == r['func_name'] and l['arguments'] == r['arguments']:
        #if l['left'] != r['left']:
        return True
            # rewrite it more carefully (for certain nodes)
    return False

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
        (graph, mapping), _ = prepare_custom_markup(scenario)

    elif (sys.argv[1]=="--test" and sys.argv[2]=="mem_anti"):
        config = {
            "working_dir" : os.path.join(current_path, "../front"),
            "src" : "1.c",
            "ir_dotfile" : "m.dot",
            "ir_pic" : "pic25.png",
            "ir_markup" : 'p_markup.png',
        }

        scenario = {
                    'type':'flowlists',
                    'data':{'yes_df_list': [
                                                
                                            ],
                            'no_df_list' : 
                            [
                                [ # relations for df no pattern #i
                                             # relation
                                                lexer.glex.Relation(
                                                left={'type': "assign*"},right={'type': "assign*"}, # src and dst nodes of relation edge
                                                predicate=check_ref_dep_mem, # function-specializer
                                                extra={"disable_labeling":
                                                            "$label $src:left" # extra labeling fmt string
                                                },
                                                label="DF_dep_from", # default label
                                                params={"edge_style": {"color": "#f76d23"}})# parameters for edge visualizing
                                                ,
                                                lexer.glex.Relation(
                                                left={'type': "assign_function_call"},right={'type': "assign_function_call"}, # src and dst nodes of relation edge
                                                predicate = check_dup_func, # function-specializer
                                                extra={"disable_labeling":
                                                            "$label $src:left" # extra labeling fmt string
                                                },
                                                label="DF_dep_free", # default label
                                                params={"edge_style": {"color": "#f00083"}})# parameters for edge visualizing
                                ]
                                             
                            ],
                            'yes_cf_list': [["any if_cond", "if_cond any",  "assign_function_call any", "any assign_function_call"]], #
                            'no_cf_list' : [["return_val exit"]],
                            'rel_kinds'  : set()}
                    }
        # phase == frontend
        (graph, mapping), working_dir = prepare_custom_markup(scenario, config)
        print("Mapping:", mapping)

        prep_core_inp_file = os.path.join(working_dir, "prepr_core_input.txt")

        def emit_grammar(gr=[['S','AB'],['A','b'],['B','c']], path="1.txt"):
            with open(path, "w") as f:
                f.write("1\n")
                f.write(str(len(gr))+"\n")
                for item in gr:
                    for i in item:
                        f.write(i+" ")
                f.write("\n\n")
            return

        def normalize_name(nm):
            nm = nm.replace(" ", "_")
            return nm[1:-1] if nm.startswith("\"") and nm.endswith("\"") else nm

        def ba_list_text(strlist, nodes):
            ret_list = []
            for row in strlist:
                ret_list.append(
                    [
                        normalize_name(nodes[int(row[i])]) for i in range(len(row))
                    ]
                    )
            return ret_list

        def emit_txt_graph(graph, path):

            with open(path,"a+") as f:
                #TO DO: implement translation
                nodes_list = [normalize_name(n.get_name()) for n in graph.get_nodes()]
                adj_list = []
                print("nodes:", nodes_list)
                for e in graph.get_edges():     
                    adj_list.append((nodes_list.index(normalize_name(e.get_source())), 
                                    nodes_list.index(normalize_name(e.get_destination())), 
                                    e.get_attributes()['label']))

                f.write(str(len(nodes_list))+ "  " + str(len(adj_list)) +"\n")
                for n in nodes_list:
                    f.write(n + " ")
                f.write("\n")
                for e in adj_list:
                    f.write(str(e[0])+ "  " + str(e[1]) + " " + str(e[2]) + "\n")

            return path, nodes_list

        emit_grammar(path=prep_core_inp_file)
        _, nodes = emit_txt_graph(graph, prep_core_inp_file)

        # configure - move it to separate function
        abs_path = os.path.join(os.getcwd(), ".." + os.sep + "front")
        abs_path_core = os.path.join(abs_path, "build" + os.sep + "core")

        # phase == core
        def run_core(core_path, grammar=None):
            print("Running core on file:", core_path)
            result = subprocess.run(
            [core_path, prep_core_inp_file], capture_output=True, text=True)
            res_stdout = result.stdout
            res_stderr = result.stderr
            return res_stdout, res_stderr

        res_stdout, res_stderr=run_core(abs_path_core)
        
        # graph annotation
        def process_core_output(s, graph=None):
            strlist = s.split('\n')
            strlist = [n.split(" ")[:-1] if n.split(" ")[-1] == "" else n.split(" ") for n in strlist]
            strlist = [n  for n in strlist if len(n) >= 2]
            # alternative: strlist = [n.split(" ") if n.split(" ")[-1] != "" for n in strlist]
            
            return strlist

        res_out = process_core_output(res_stdout)
        ba = ba_list_text(res_out, nodes)
        pprint_path = ""
        for entry in ba:
            pprint_path += " --> ".join(entry) + "\n"
        
        print(pprint_path)

        # optionally: graph painting
        nodes_set = graph.get_nodes()
        res_graph = graph

        for n in nodes_set:
            if (normalize_name(n.get_name()) in [item for sublist in ba for item in sublist]):
                n.set_color('lavender')
                n.set_style('filled')

        def sublist(lst1,lst2):
            for item in lst2:
                try:
                    lst1.index(item)
                except ValueError:
                    return False
                return True
        # TO DO: rewrite it more light-weightly
        for e in res_graph.get_edges():
            if res_graph.get_node(e.get_source())[0].get_style() == 'filled' and \
            res_graph.get_node(e.get_destination())[0].get_style() == 'filled' and sublist(ba, [normalize_name(e.get_source()),normalize_name(e.get_destination())]):

                e.set_color('violet')
        

        res_graph.write_png(os.path.join (config["working_dir"],"detected_pathes_visualization.png"))

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

       

        
