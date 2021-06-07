# pathfinder
CFL-R-based software analysis tool
# ResearchGate:
1) https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis
# Build


1) cd build
2) cd gcc-cfg-utils
3) cmake ./
4) make
5) bash run.sh

When running "bash run.sh" it starts to work with code analysis. For test it is needed to write "../examples/test1.c", then write grammar rules (first: number of rules, then rules.) <br>For example:

	5 
	S AB
	S AR
	R SB
	A a
	B b
	
<br> Then to put letters on edges. <br><br>

It produces file build/core, which is core executable of CFL-R analysis and
can be called by frontend (see example in front/callgrind-front).
