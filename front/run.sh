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
fi