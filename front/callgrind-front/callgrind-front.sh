#!/usr/bin/env bash

# usual pipeline example for gprof:
# gprof ./main | gprof2dot -s | dot -Tpng -o output.png
pwd
python3 ../../third-party/gprof2dot/gprof2dot.py ${1} --format=callgrind -c print -o ${2}