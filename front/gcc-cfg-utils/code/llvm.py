import sys
import subprocess
import os
graph = open("data/graph", "w")
graph.write(str(len(sys.argv) - 1) + "\n") #print number of functions

for file in range(1, len(sys.argv)):
    if file != 1:
        graph.write("\n")
    print("Analyzing file " + sys.argv[file])
    data = open(sys.argv[file]).readlines()
    nodes, edges, rules, blocks_of_code = [], [], [], {}

    for i in data:
        a, b, c = i.find("["), i.find("->"), i.find(":")
        if a != -1:
            blocks_of_code.update([(i[1:a-1], i[i.find("{")+1:-4])])
            nodes.append(i[1:a-1])
        if b != -1:
            if c < b and c != -1:
                b = c + 1
            edges.append((i[1:b-1], i[i.find("->") + 3:-2]))

    n = int(input("Specify the number of rules: "))
    graph.write(str(n) + "\n")

    #input rules
    for i in range(n):
        rules.append(raw_input().split())
        graph.write(rules[i][0] + " " + rules[i][1] + "\n")
    
    #output
    graph.write("\n" + str(len(edges)) + " " + str(len(edges)) + "\n") #write number of edges
    for i in nodes:
        graph.write(i + " ")
    graph.write("\n")
    for i in range(len(edges)):
        graph.write(str(nodes.index(edges[i][0])) + " " + str(nodes.index(edges[i][1])) + " " + raw_input(str(edges[i])) + " " + "\n")

#proc = subprocess.call(["ls", "-la"])
graph.close()
path_to = os.path.abspath(sys.argv[0])[:os.path.abspath(sys.argv[0]).find("code")]
proc = subprocess.call([path_to + "build/core", path_to + "build/"])