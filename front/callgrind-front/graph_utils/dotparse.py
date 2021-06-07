import pydot
import sys

#fmt:


def labels_encode(edges, labels=[]): # this is a hash-function for edge labeling, rewrite it as you need
    d = dict()
    for idx,l in enumerate(edges):
        if l.__get_attribute__('label') in labels:
            d[l.__get_attribute__('label')] = chr(ord('a')+idx)#l.get_source() + l.get_destination()

    return d


def node_names_encode(labels=[]):
    d = dict()
    for idx,l in enumerate(labels):
        d[l] = idx

    return d


def verbose_print_edge(idx, e,lmap):
    print("edge #", idx)
    print("from: ", e.get_source())
    print("to: ", e.get_destination())
    print("old label: ", e.__get_attribute__('label'))
    print("set label: ", lmap[e.__get_attribute__('label')] if e.__get_attribute__('label') in lmap.keys() else "")
    return


def dotparse(inp, outp=None, labels=None, verbose=True, nodes_remap=True, edges_remap=True):
    graph = pydot.graph_from_dot_file(inp)
    node_list = graph[0].get_node_list()
    if not labels:
        labels = [el.__get_attribute__('label') for el in graph[0].get_edges()]
    node_labels_map = node_names_encode([nl.get_name() for nl in node_list])
    lmap = labels_encode(graph[0].get_edges(), labels)

    if outp:
        f = open(outp, "w")
        nodes_amount = len(node_list)
        edges_amount = len(graph[0].get_edges())
        f.write(str(nodes_amount)+" "+str(edges_amount)+"\n")
    for idx, e in enumerate(graph[0].get_edges()):
        attr_orig = e.__get_attribute__('label')  # NB: you also can populate labels with this info!
        if verbose:
            verbose_print_edge(idx, e, lmap)
        # Triple of sets <$src$ \in $V$, $dst$ \in $E$, $current_label$ \in $L$> is all we need
        src = e.get_source()
        dst = e.get_destination()
        current_label = lmap[attr_orig] if attr_orig in labels else "n" # "" means "not interested in this label"
        if outp:
            f.write(str(node_labels_map[src]) + " " + str(node_labels_map[dst]) + " " + current_label+"\n")
    if outp:
        f.close()
        f=open(outp+".labels_remap","w")
        f.write(str(edges_amount)+"\n")
        for k,v in lmap.items():
            f.write(k+" "+str(v)+"\n")
        f.write(str(nodes_amount) + "\n")
        for k,v in node_labels_map.items():
            f.write(k+" "+str(v)+"\n")

    return


if __name__ == "__main__":
    if sys.argv[1] == 'test' or len(sys.argv) < 2:
        dotparse("/Users/nefanov/PycharmProjects/pathfinder/front/callgrind-front/tests/callgrind.out.2322.dot",
             labels=[],
             verbose=False,
             outp="labeled_edges_list.txt")
    else:
        dotparse(sys.argv[1],
                 verbose=False,
                 outp=sys.argv[2])

