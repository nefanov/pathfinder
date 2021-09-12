import random
import sys
fout = open("graph_generator.output", "w")
fout.write("1\n5\n")
fout.write("S AB \nS AR \nR SB \nA a \nB b\n")
type = int(sys.argv[1]) #type of graph by density
n = int(sys.argv[2])    #number of vertices
if type == 0:
    m = 2*n
if type == 1:
    m = int(n**1.5)
if type == 2:
    m = n**2/4
if type == 3:
    m = n*(n-1)
fout.write("\n")
fout.write(str(n) + " " + str(m) + "\n")
for i in range(n):
    fout.write("T ")
fout.write("\n")
A = ['a', 'b', 'c']
if type != 3:
    for i in range(m):
        t1 = 0
        t2 = 0
        while t1 == t2:
            t1 = random.randint(0, n-1)
            t2 = random.randint(0, n-1)
        fout.write(str(t1) + " " + str(t2) + " " + A[random.randint(0,2)] + "\n")
else:
    for i in range(n):
        for j in range(n):
            if i != j:
                fout.write(str(i) + " " + str(j) + " " + A[random.randint(0, 2)] + "\n")