#!/bin/bash
if [[ $0 == "run.sh" ]]; then
    path="./"
else
    path=${0%run.sh}
fi
if [[ $1 == "-test" ]]; then
    if [[ $0 == "run.sh" ]]; then
        build/test
    else
        cd $path
        build/test
    fi
else
    if [[ $1 == "-llvm" ]]; then
        python $path/code/llvm.py $*
    else
        $path/build/code2graph $*
    fi
fi