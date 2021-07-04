# GCC Analyzer
# Run
Firstly, create a dot file for analyzed code:

    gcc -fdump-tree-cfg [filename]

Then, run analyzer:<br>(specify path to `run.sh` correctly if you run it not from here)

    bash ../run.sh
    or
    bash ../run.sh [path to .dot file]
    or
    bash ../run.sh -file [path to input file] (e.g input/example.in)

You can run unit-tests by

    bash ../run.sh -test

# Description