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
