cd build
if [[ $1 == "test" ]]; then
    ./test
else
    ./code2graph $1
fi