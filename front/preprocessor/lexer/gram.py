def make_gram_map(labels, m={}, any_any=True):
    ASCII=''.join(chr(i) for i in range(256))
    idx = 0
    [m.update({labels[i]:ASCII[i+97]}) for i in range(len(labels))]
    if any_any:
        m.update({"any any":'z'})
    return m


def prepare_txt_grammar(grammar=list(), filename="graph_core_inp.txt"):
    f = open(filename, "w+")
    num_of_rules = len(grammar)
    f.write(str(num_of_rules)+'\n')
    for l in grammar:
        f.write(l+'\n')
    f.write('\n')
    f.close()
    return


def prepare_txt_graph(graph, filename="graph_core_inp.txt"):
    f = open(filename,"a+")
    V = graph.get_nodes()
    E = graph.get_edges()
    V_pow = len(V)
    E_pow = len(E)
    f.write(str(V_pow) + " " + str(E_pow)+'\n')
    V_names = {}
    for i, item in enumerate(V):
        V_names.update({item.get_name():i})
        f.write(item.get_name()+" ")
    f.write('\n')

    #print(V_names.keys())
    for e in E:
        src_name = e.get_source()
        dst_name = e.get_destination()
        lbl = e.get_attributes()['label']
        f.write(str(V_names[src_name])+" "+str(V_names[dst_name])+" "+lbl+"\n")

    f.close()
    return V_names


def reverse_map(m):
    res={}
    for k,v in m.items():
        res.update({v:k})
    return res