#!/bin/sh
cd third-party
git clone https://github.com/JetBrains-Research/GLL4Graph.git
cd GLL4Graph
mkdir results
export JAVA_HOME='/usr/lib/jvm/jdk-15.0.2'
