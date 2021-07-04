# GCC Analyzer
# Run
Firstly, create a dot file for analyzed code:

    gcc -fdump-tree-cfg [filename]

Then, run analyzer:<br>(you can specify path to `start.sh` or to `run.sh` if you want)

    bash start.sh
    or
    bash start.sh [path to .dot file]
    or
    bash start.sh -file [path to input file] (e.g input/example.in)

You can run unit-tests by

    bash start.sh -test

# Description