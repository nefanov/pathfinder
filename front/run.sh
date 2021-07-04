#!/bin/bash
path=${0/run.sh/"./"}
path=${path/start.sh/"../"}
if [[ $1 == "-test" ]]; then
    cd $path
    build/test
else
    $path/build/code2graph $@
fi