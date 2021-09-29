import graphlex as glex
from predicates import *

def compose_mem_pattern(scenario={}):
    if not scenario:  # if scenario is not preset
        P = {'yes_df_list': [], 'no_df_list': [], 'yes_cf_list': [], 'no_cf_list': [], 'rel_kinds': set()}
        # first for good paths
        rel = glex.Relation(left={'type': "assign_function_call"},
                       right={'type': "assign_function_call"},
                       predicate=predicate_malloc_memset,
                       extra=None,
                       label="DF_malloc_memset",
                       params={"edge_style": {"color": "dodgerblue"}})
        P['yes_df_list'].append([rel])
        P['rel_kinds'].add(rel)
        P['yes_cf_list'].append(
            ["assign_function_call any", "any assign_function_call", "assign_function_call assign_function_call"])

        # then for forbridden paths
        no_rel = glex.Relation(left={'type': "assign_function_call"},
                          right={'type': "assign_aryphmetic_op"},
                          predicate=predicate_malloc_getptr,
                          extra=None,
                          label="DF_malloc_getptr",
                          params={"edge_style": {"color": "dodgerblue"}})

        no_rel_next = glex.Relation(left={'type': "assign_aryphmetic_op"},
                               right={'type': "assign_const"},
                               predicate=predicate_getptr_write,
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
                if not p.extra and p.predicate(n1s['format'], n2s['format']) and shortest_path_check(graph, n, n2):
                    graph.add_edge(pydot.Edge(n,
                                    n2,
                                    color=p.params["edge_style"]["color"],
                                    style ='dashed',
                                    label=p.label))
    return graph
