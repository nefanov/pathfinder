#!/bin/bash
absolute_path=`readlink -e "$0"`
absolute_path=`dirname "$absolute_path"`
absolute_path=${absolute_path%/gcc-cfg-utils}
echo $absolute_path
if [[ $1 == "-test" ]]; then
    cd $absolute_path
    build/test
else
    $absolute_path/build/code2graph $@ -front-only
    $absolute_path/build/core $absolute_path/data/graph $@
fi