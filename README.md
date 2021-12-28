# pathfinder
CFL-R-based software analysis tool
# ResearchGate:
1) [Modern formal languages theory based methods for software static analysis](https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis)

# Reference:
If you use Pathfinder, please, cite this article:

Н. Н. Ефанов, А. Р. Федоров, Э. В. Шамбер, А. А. Щербаков, А. П. Елесина Pathfinder: статический анализатор программ на базе решения задач достижимости на графах в КС-ограничениях // Труды МФТИ, Том 13,  №4, 2021, сс 14-29.

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
# Run
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

# LLVM IR

Also it is supported llvm compilers<br>
To analyze LLVM IR bitcode you should switch to LLVM branch:

	git checkout llvm
