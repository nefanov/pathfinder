The lexer also provides grammar input functionality: 
input of grammar for the core module now is enumerative for each edge as a pair of connected nodes, so preparing should be related to such pairs sequence.

Note: current lexer is dependent on GCC (it tokenizes GIMPLE format)

# Requirements
* GCC >= 9 
* Python 3 + extra packages:
-- PyDot
-- Networkx
-- JSON (optional, for verbose output)
* GraphViz (currently not need PyGraphViz)
