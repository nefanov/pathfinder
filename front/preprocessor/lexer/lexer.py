import json
import graphlex as glex
from predicates import *
import gram as gr


def prepare_graph(graph_fn, outp_graph_fn, outp_plot, need_graph_save=False, need_plot=False, pattern_composer=None, specializer=None, verbose=False, scenario=None):
    inp_file = graph_fn if graph_fn else "mem.c.012t.cfg.dot"
    outp_pic = outp_plot if outp_plot else "g_out.png"
    outp_graph = outp_graph_fn if outp_graph_fn else "g_out.dot"
    if not pattern_composer:
        pattern_composer = glex.default_pattern_composer
    if not specializer:
        specializer = glex.default_specializer

    def flatten(t):
        return [item for sublist in t for item in sublist]

    # markup nodes and edges by token types
    graph, nodes, nld = glex.lex_graph(inp_file)

    # add labeled extra edges, check some predicates; return graph, pattern description list
    graph, P = glex.markup_graph(graph, nodes, nld, pattern_composer=pattern_composer, scenario=scenario, specializer=specializer)
    if need_plot:
        graph.write_png(outp_pic+"_pre.png")
    # create mapping of graph (edges_types --> terminal alphabet)
    l_1 = list(set([p.label for p in flatten(P['yes_df_list']) + flatten(P['no_df_list'])]))
    l_2 = list(set([p for p in flatten(P['yes_cf_list'])]))
    l_3 = list(set([p for p in flatten(P['no_cf_list'])]))
    if verbose:
        print("DEBUG: function", __name__, "l 1-3", l_1,l_2,l_3)
    mapping = gr.make_gram_map(sorted
                               (l_1
                                +
                                l_2
                                +
                                l_3
                                ))
    # markup edges by terminal alphabet
    graph = glex.markup_edges(graph, mapping)
    if verbose:
        print("============MAPPING============")
        print(mapping)

    if need_plot:
        graph.write_png(outp_pic)
    return graph, mapping


def prepare_grammar_files(graph, G_list=[], verbose=False, file_name_prefix="grammar"):
    """
    prepare grammars: one file for each grammar
    Note: it is graph-specific because CFL-R input is enumerative (see the core module sources for details)
    """
    files_list = []
    for i, grammar in enumerate(G_list):
        file_name = file_name_prefix + str(i)
        files_list.append(file_name)
        gr.prepare_txt_grammar(
            grammar, file_name)
        saved_node_names = gr.prepare_txt_graph(graph, file_name)
        if verbose:
            print(saved_node_names)

    if verbose:
        print("Generated grammar input files for core:\n", files_list)
    return
