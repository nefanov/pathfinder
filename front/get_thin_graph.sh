gcc -fdump-tree-cfg-graph $1
echo $1 | build/code2graph -front-only -thin > $2
rm $1.012t.cfg*
