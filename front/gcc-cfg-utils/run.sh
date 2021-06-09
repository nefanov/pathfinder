if [[ $1 == "-test" ]]; then
    cd ${0%run.sh}
    build/test
else
    ${0%run.sh}/build/code2graph $1
fi