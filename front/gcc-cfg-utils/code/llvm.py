import sys
data = open(sys.argv[1]).readlines()
l1, l2, rules, d1 = [], [], [], {}
for i in data:
    a, b, c = i.find("["), i.find("->"), i.find(":")
    if a != -1:
        d1.update([(i[1:a-1], i[i.find("{")+1:-4])])
        l1.append(i[1:a-1])
    if b != -1:
        if c < b and c != -1:
            b = c + 1
        l2.append((i[1:b-1], i[i.find("->") + 3:-2]))
n = int(input())
graph = open("data/graph_llvm", "w")
graph.write("1\n" + str(n)+"\n")
for i in range(n):
    rules.append(raw_input().split())
    graph.write(rules[i][0] + " " + rules[i][1] + "\n")
graph.write("\n" + str(len(l2)) + " " + str(len(l2)) + "\n")
for i in range(len(l2)):
    graph.write(str(l1.index(l2[i][0])) + " " + str(l1.index(l2[i][1])) + " " + raw_input(str(l2[i])) + " " + "\n")