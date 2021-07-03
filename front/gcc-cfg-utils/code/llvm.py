import sys
import subprocess
import os

graph = open("data/graph", "w")
if sys.argv[1] == "-llvm":
    if sys.argv[2] == "-file":
        input_data = open(sys.argv[3]).readlines()
        extraargs, k, is_file = 4, 0, True
    else:
        extraargs, k, is_file = 2, 0, False
else: 
    extraargs, k, is_file = 1, 0, False
graph.write(str(len(sys.argv) - extraargs) + "\n") #print number of functions
print(sys.argv)
for fnumber in range(extraargs, len(sys.argv)): #analyze function with number fnumber
    if fnumber != extraargs:
        graph.write("\n")
    print("Analyzing file " + sys.argv[fnumber])
    data = open(sys.argv[fnumber]).readlines()
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

    n = int(input("Specify the number of rules: ")) if is_file == False else int(input_data[k])
    graph.write(str(n) + "\n")
    k+=1

    #input rules
    for i in range(n):
        rules.append(raw_input().split()) if is_file == False else rules.append(input_data[k].split())
        graph.write(rules[i][0] + " " + rules[i][1] + "\n")
        k+=1
    
    #output
    graph.write("\n" + str(len(edges)) + " " + str(len(edges)) + "\n") #write number of edges
    for i in nodes:
        graph.write(i + " ")
    graph.write("\n")
    for i in range(len(edges)):
        letters = raw_input(str(edges[i])) + "\n" if is_file == False else input_data[k]
        k += 1
        graph.write(str(nodes.index(edges[i][0])) + " " + str(nodes.index(edges[i][1])) + " " + letters)

graph.close()
path_to = os.path.abspath(sys.argv[0])[:os.path.abspath(sys.argv[0]).find("code")]
proc = subprocess.call([path_to + "build/core", path_to + "build/"])