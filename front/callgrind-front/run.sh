#!/usr/bin/env bash
#test run example:      0               1           2                   3               4           5
# whole pipeline: ./callgrind-front.sh <profile> <profile_output_dot> <grammar_file> <parsed_dot> <res_file>

./callgrind-front.sh tests/callgrind.out.1972 res.list tests/test.grammar res.inp core.inp