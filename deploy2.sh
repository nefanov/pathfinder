#!/bin/sh
cd third-party
sudo apt install maven
git clone https://github.com/JetBrains-Research/GLL4Graph.git
cd GLL4Graph
mkdir results
export JAVA_HOME='/usr/lib/jvm/jdk-15.0.2'
mvn compile
cd ..
cd ..
cd front
cmake ./
make
cd ..
cd third-party/GLL4Graph/data/core
cp core_nodes.csv core_all_nodes.csv
