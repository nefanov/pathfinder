#!/usr/bin/env bash

# usual pipeline example for gprof:
# gprof ./main | gprof2dot -s | dot -Tpng -o output.png
# this script usage example:
# ./callgrind-front.sh tests/callgrind.out.2322 result.dot
# produces result.dot and result.dot.png in current working directory

pwd
python3 ../../third-party/gprof2dot/gprof2dot.py ${1} --format=callgrind -c print -o ${2}
dot ${2} -Tpng -o ${2}.png