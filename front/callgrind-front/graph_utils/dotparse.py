import pydot
import sys

#fmt:

def dotparse(inp, outp="res.txt", verbose=True):
    graph = pydot.graph_from_dot_file(inp)
    node_list = graph[0].get_node_list()
    for idx, e in enumerate(graph[0].get_edges()):
        if verbose:
            print("edge #", idx)
            print("from: ", e.get_source())
            print("to: ", e.get_destination())
            print("label: ", e.__get_attribute__('label'))
    return

if __name__=="__main__":
    dotparse("/Users/nefanov/PycharmProjects/pathfinder/front/callgrind-front/tests/callgrind.out.2322.dot")

