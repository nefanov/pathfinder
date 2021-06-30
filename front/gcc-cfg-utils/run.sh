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
    $path/build/code2graph $1 $2
fi