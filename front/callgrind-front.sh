#!/usr/bin/env bash

# usual pipeline example for gprof:
# gprof ./main | gprof2dot -s | dot -Tpng -o output.png
# this script usage example:
# ./callgrind-front.sh tests/callgrind.out.2322 result.dot
# produces result.dot and result.dot.png in current working directory

# whole pipeline: ./callgrind-front.sh <profile> <profile_output_dot> <grammar_file> <parsed_dot> <res_file>
pwd
#produces dotfile
python3 ../../third-party/gprof2dot/gprof2dot.py ${1} --format=callgrind -c print -o ${2}
#parse dotfile and compose input for core algorithm (${3} is the path to your grammar file)
python3 graph_utils/dotparse.py ${2} ${4}
cat ${3} ${4} > ${5}
#run core
../../core/build/core < ${5}
#produces image file
dot ${2} -Tpng -o ${2}.png
