#!/usr/bin/env bash
# script-getter of thin CFG from GCC frontend (currently it is our main frontend)
# example of usage:
# suppose you want to get thin-bb graph of 1.c and save it to dot for CFL-R and to png for manual analysis
# then you should perform:
# > cd front
# > ./get_thin_graph.sh -i 1.c -s 123.dot -p pic.png
#  or, if you need SSA,
# > ./get_thin_graph.sh -i 1.c -s 123.dot -p pic.png --ssa 
# stdout: Picture generating from dotfile 123.dot, saving to: pic.png

# functionality:

POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
    -i|--input)
      SRC="$2"
      shift # past argument
      shift # past value
      ;;
    -s|--save_out)
      OUTPUT="$2"
      shift # past argument
      shift # past value
      ;;
    -p|--plot)
      PLOTFN="$2"
      shift # past argument
      shift # past value
      ;;
    --ssa)
      SSA_MODE=YES
      shift # past argument
      ;;     
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
      ;;
  esac
done

FRONT_CMD_SUFFIX=""
GCC_CMD=-fdump-tree-cfg-graph
if [[ $SSA_MODE == YES ]]; then
    GCC_CMD=-fdump-tree-ssa-graph
    FRONT_CMD_SUFFIX=-SSA
fi


gcc-9 $GCC_CMD $SRC && sync ${SRC}.*t.*.dot
echo $SRC | build/code2graph -cmd $SRC -front-only -thin $FRONT_CMD_SUFFIX > ${SRC}_gcc_frontend.output && sync ${SRC}_gcc_frontend.output
csplit ${SRC}_gcc_frontend.output '/digraph /' > /dev/null
rm ${SRC}_gcc_frontend.output && sync
cat xx01 > $OUTPUT && sync $OUTPUT
rm xx*
sed -i '$ d' $OUTPUT && sync $OUTPUT
rm $SRC.*t.*

if [[ PLOTFN ]]; then
    echo -e "Picture generating from dotfile \033[1;34m$OUTPUT\033[0m, saving to: \033[1;31m${PLOTFN}\033[0m";
    dot -Tpng $OUTPUT -o ${PLOTFN}
    exit 0
fi
