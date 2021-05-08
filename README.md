# pathfinder
CFL-R-based software analysis tool
# ResearchGate:
1) https://www.researchgate.net/project/Modern-formal-languages-theory-based-methods-for-software-static-analysis
# Build

You need only to build the core module (another parts of project are optional):


1) cd core && mkdir build
2) cd build && cmake ..
3) make

It produces file build/core, which is core executable of CFL-R analysis and
can be called by frontend (see example in front/callgrind-front).
