# pathfinder
CFL-R-based software analysis tool
# ResearchGate:
1) https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis
# Build


1) cd build
2) cd gcc-cfg-utils
3) cmake ./
X4) make
5) bash run.sh

<<<<<<< HEAD
When running "bash run.sh" it starts to work with code analysis. For test it is needed to write "../examples/test1.c", then write grammar rules (first: number of rules, then rules.) <br>For example:
=======
1) cmake ./
2) make
3) bash run.sh

When running ***bash run.sh*** it starts to work with code analysis.
<br>For a test it is needed to write ***../front/examples/test1.c***,
<br>then write grammar rules (first: number of rules, then rules) 
<br>For example:
>>>>>>> 9274c2b6fbdd0466d840515ff76bf6f5a88dd745

	5 
	S AB
	S AR
	R SB
	A a
	B b
	
<br> Then to put letters on edges. <br>

It produces file build/core, which is core executable of CFL-R analysis and<br>
can be called by frontend (see example in front/callgrind-front).
