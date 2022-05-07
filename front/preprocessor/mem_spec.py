import os
import sys
import networkx as nx

# API paths
current_path = os.path.dirname(os.path.abspath("."))
sys.path.append(os.path.join(current_path, "../front/preprocessor/lexer"))

# API imports
import lexer


def shortest_path_check(graph, src, dst):
    nx_graph = nx.nx_pydot.from_pydot(graph)
    try:
        if nx.shortest_path(nx_graph, src, dst):
            return True
        return False
    except:
        return False


def compose_pattern(scenario=None):
    if not scenario: # default scenario
        P = {'yes_df_list': [], 'no_df_list': [], 'yes_cf_list': [], 'no_cf_list': [], 'rel_kinds': set()}
        P['yes_cf_list'].append(
            ["any if_cond", "if_cond any"])
        P['no_cf_list'].append(["return_val exit"])
    else:
        pass # scenario:#{"type":flowlists|relations, data:dict}
             # relations: set left-->right labels, predicates to check, etc
             # flowlists: *f_list of correspondent src->dst nodes' labels
        if scenario['type'] == 'flowlists':
            P = scenario['data']
        else:
            pass # to do relations

    return P


def compose_mem_pattern(scenario=None):
    if not scenario:  # if scenario is not preset
        P = {'yes_df_list': [], 'no_df_list': [], 'yes_cf_list': [], 'no_cf_list': [], 'rel_kinds': set()}
        # first for good paths
        rel = lexer.glex.Relation(left={'type': "assign_function_call"},
                       right={'type': "assign_function_call"},
                       predicate=lexer.predicate_malloc_memset,
                       extra=None,
                       label="DF_malloc_memset",
                       params={"edge_style": {"color": "dodgerblue"}})
        P['yes_df_list'].append([rel])
        P['rel_kinds'].add(rel)

        P['yes_cf_list'].append(
            ["assign_function_call any", "any assign_function_call", "assign_function_call assign_function_call"])

        # then for forbridden paths
        no_rel = lexer.glex.Relation(left={'type': "assign_function_call"},
                          right={'type': "assign_aryphmetic_op"},
                          predicate=lexer.predicate_malloc_getptr,
                          extra=None,
                          label="DF_malloc_getptr",
                          params={"edge_style": {"color": "dodgerblue"}})

        no_rel_next = lexer.glex.Relation(left={'type': "assign_aryphmetic_op"},
                               right={'type': "assign_const"},
                               predicate=lexer.predicate_getptr_write,
                               extra=None,
                               label="DF_malloc_getptr_writemem",
                               params={"edge_style": {"color": "#f76d23"}})

        P['no_df_list'].append([no_rel, no_rel_next])
        P['rel_kinds'].add(no_rel)

    else:
        pass  # TO DO: support scenarios
    return P


def specialize_Dflow(graph, nodes, node_lex_dict, P):
    """
    # nodes: словарь по типам вершин
    # P -- list(patterns (relation ))
    """
    for p in P: # for each entity (relation, edge) of pattern
        for n in nodes[p.left['type']]:
            n1s = node_lex_dict[n]['content']
            for n2 in nodes[p.right['type']]:
                n2s = node_lex_dict[n2]['content']
                # TO DO: check scope!
                if not p.extra and p.predicate(n1s['format'], n2s['format'],None) and shortest_path_check(graph, n, n2):
                    graph.add_edge(pydot.Edge(n,
                                   n2,
                                   color=p.params["edge_style"]["color"],
                                   style ='dashed',
                                   label=p.label))
    return graph


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
