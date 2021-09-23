# example of usage:
# suppose you want to get thin-bb graph of 1.c and save it to dot for CFL-R and to png to manual analysis
# then you should perform:
# > cd front
# > ./get_thin_graph.sh 1.c 123.dot pic.png
#
# stdout: Picture generating from dotfile 123.dot, saving to: pic.png

# functionality:
gcc -fdump-tree-cfg-graph $1 && sync $1.012t.cfg*
echo $1 | build/code2graph -cmd $1 -front-only -thin > $1_gcc_frontend.output && sync $1_gcc_frontend.output
csplit $1_gcc_frontend.output '/digraph /' > /dev/null
rm $1_gcc_frontend.output && sync
cat xx01 > $2 && sync $2
rm xx*
sed -i '$ d' $2 && sync $2
rm $1.012t.cfg*

if [[ $# -eq 3 ]]; then
    echo -e "Picture generating from dotfile \033[1;34m$2\033[0m, saving to: \033[1;31m$3\033[0m";
    dot -Tpng $2 -o $3
    exit 0
fi
