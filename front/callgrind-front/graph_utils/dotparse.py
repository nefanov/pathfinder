import pydot
import sys

#fmt:


def labels_encode(labels=[]): # this is a hash-function for edge labeling, rewrite it as you need
    d = dict()
    for idx,l in enumerate(labels):
        d[l] = "a" + str(l)

    return d


def verbose_print_edge(idx, e,lmap):
    print("edge #", idx)
    print("from: ", e.get_source())
    print("to: ", e.get_destination())
    print("old label: ", e.__get_attribute__('label'))
    print("set label: ", lmap[e.__get_attribute__('label')] if e.__get_attribute__('label') in lmap.keys() else "")
    return


def dotparse(inp, outp=None, labels=["e1","e2","e3"], verbose=True):
    lmap = labels_encode(labels)
    graph = pydot.graph_from_dot_file(inp)
    node_list = graph[0].get_node_list()
    nodes_amount = len(node_list)
    edges_amount = len(graph[0].get_edges())
    for idx, e in enumerate(graph[0].get_edges()):
        attr_orig = e.__get_attribute__('label')  # NB: you also can populate labels with this info!
        if verbose:
            verbose_print_edge(idx, e, lmap)
        # Triple of sets <src \in V, dst \in E, current_label \in L> is all we need
        src = e.get_source()
        dst = e.get_destination()
        current_label = lmap[attr_orig] if attr_orig in labels else "" # "" means "not interested in this label"
    return


if __name__=="__main__":
    dotparse("/Users/nefanov/PycharmProjects/pathfinder/front/callgrind-front/tests/callgrind.out.2322.dot",
             labels=[], verbose=False, outp="labeled_edges_list.txt")

