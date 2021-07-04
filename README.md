# pathfinder
CFL-R-based software analysis tool
# ResearchGate:
1) [Modern formal languages theory based methods for software static analysis](https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis)

# Requirements
* GCC 9.3.0
* CMAKE 3.16.3
* CLANG 10.0.0 *(only for llvm analysis)*
* LLVM 10.0.0 *(only for llvm analysis)*
* PYTHON 2.7.18 *(only for llvm analysis)*
# Build


	cd front/gcc-cfg-utils
	cmake ./
	make
	bash run.sh [path to file]

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

Also it is supported llvm compilers<br>
To analyze LLVM IR bitcode use:

	clang -S -emit-llvm [filename.c]
	opt -dot-cfg [filename.ll]
	---------------------------------------
	python code/llvm.py [.function1.dot] [.function2.dot] ...
	or
	bash run.sh -llvm [.function1.dot] [.function2.dot] ...
	or 
	bash run.sh -llvm -file [input/llvm.in] [.function1.dot] [.function2.dot] ...