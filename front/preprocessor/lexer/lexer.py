import json
import graphlex as glex
from predicates import *
import gram as gr


def prepare_graph(graph_fn, outp_graph_fn, outp_plot, need_graph_save=False, need_plot=False, P=None):
    inp_file = graph_fn if graph_fn else "mem.c.012t.cfg.dot"
    outp_pic = outp_plot if outp_plot else "g_out.png"
    outp_graph = outp_graph_fn if outp_graph_fn else "g_out.dot"

    def flatten(t):
        return [item for sublist in t for item in sublist]

    # markup nodes and edges by token types
    graph, nodes, nld = glex.lex_graph(inp_file)
    # add labeled extra edges, check some predicates
    graph, P = glex.markup_graph(graph, nodes, nld)  # return graph, pattern description list
    if need_plot:
        graph.write_png(outp_pic)
    # create mapping of graph (edges_types --> terminal alphabet)
    mapping = gr.make_gram_map(sorted
                               (list(set([p.label for p in flatten(P['yes_df_list']) + flatten(P['no_df_list'])]))
                                + list(set([p for p in flatten(P['yes_cf_list'])]))
                                ))
    # markup edges by terminal alphabet
    graph = glex.markup_edges(graph, mapping)
    if need_plot:
        graph.write_png("mapped2_" + inp_file + outp_pic)
    return graph, mapping
