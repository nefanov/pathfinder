# pathfinder
CFL-R-based software analysis tool

## Reference:
If you use Pathfinder, please, cite this article:

Н. Н. Ефанов, А. Р. Федоров, Э. В. Шамбер, А. А. Щербаков, А. П. Елесина Pathfinder: статический анализатор программ на базе решения задач достижимости на графах в КС-ограничениях // Труды МФТИ, Том 13,  №4, 2021, сс 14-29. DOI:10.53815/20726759_2021_13_4_14

Or:

Efanov, N., Fedorov, A.R., Shamber, E.V., Shcherbakov, A.A., & Elesina, A.P. (2021). Pathfinder: software static analyzer based on solving the graph reachability problem with CF constraints. Proceedings of Moscow Institute of Physics and Technology. Vol. 13, 4 pp. 14-29. (In Russian) DOI:10.53815/20726759_2021_13_4_14 

## Reports, talks:

**Apr. 2022:**

N. Efanov, S. Efimov // Pathfinder: overview of the tool, approach & applications. YC Seminar, SPbSU: https://drive.google.com/file/d/1bEjzNo4M-ER4jznoXGKloXfuwJeKLXC2/view

N. Efanov, S. Efimov // CFL-R approach for code static analysis. Techtalk on KMTT Seminar, MIPT: 
https://youtu.be/EDp_dERI1GM

**Jun. 2022:**

S. Efimov // Static analysis of cycles vectorization based on CFL-reachability. Bachelor degree defence, MIPT, KMTT Dept.

## ResearchGate:
1) [Modern formal languages theory based methods for software static analysis](https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis)

# Requirements
* GCC 9.3.0
* CMAKE 3.16.3
* PYTHON 3.7 *(only for Lexer and Callgrind-front analysis)*
* CLANG 10.0.0 *(only for llvm analysis)*
* LLVM 10.0.0 *(only for llvm analysis)*
* PYTHON 2.7.18 *(only for llvm analysis)*
# Build
	cd front
	cmake ./
	make
# Run [in demo mode, with simplified grammar rules]
In [path to file] you need to specify the path to the file pre-compiled<br>with `gcc file_name -fdump-tree-cfg-graph`<br>
If you omit it, the program will ask you to specify it via stdin<br><br>

When running `bash run.sh` it starts to work with code analysis.
<br>For a test it is needed to write `examples/test1.c` (if you are in front/gcc-cfg-utils and not using cmd args),
<br>then write grammar rules (first: number of rules, then rules) 
<br>For example:

	5 
	S AB
	S AR
	R SB
	A a
	B b

<br> Then to put letters on edges. <br>

It produces file build/core, which is core executable of CFL-R analysis and<br>
can be called by frontend (see example in front/callgrind-front).
<br><br>
If you want to run the unit-tests you need write:
	
	bash run.sh -test

It is also supported file input method<br>
To use it write:

	bash run.sh -file [path to file] (For example: input/example.in)

# Supported flags
*	`-cmd` the next argument should be the path to analyzed file
*	`-file` the next argument should be the path to input file
*	`-front-only` to not run the core file, only to generate data/graph
*	`-thin` to present data/graph string by string (`thin-bb version`)
*	`-fast` to use `fast.cpp` alghorithm
*	`-slow` to use `slow.cpp` alghorithm
*	`-test` to run unit-tests
*	`-o` 	to save front output into specific file (specify path into next argument)
*	`-ssa` 	to represent input graph in SSA form
# Examples
*		bash run.sh
		gcc-cfg-utils/examples/test1.c
		1
		S a
		a a b b a a a
*		bash run.sh -cmd gcc-cfg-utils/examples/test1.c
		1
		S a
		a a b b a a a
*		bash run.sh -file gcc-cfg-utils/input/example.in

# Run [in general mode]
<br>The terms in grammar rules can be spaced strings, just pass -spaced_rhs as option for core. For example:

*	StrTerm StrTermRhs1 StrTermRhs2

# LLVM IR

Also it is supported llvm compilers<br>
To analyze LLVM IR bitcode you should switch to LLVM branch:

	git checkout llvm
	
# Backends
1) Melski-Reps, CYK -- based: 
- "slow" mode
- "fast" mode
2) GLL-based:
- Iguana (GLL4Graph): See in documentation_GLL4Graph document
