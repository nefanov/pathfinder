#!/bin/sh
cd third-party
sudo apt install maven
git clone https://github.com/LednevaAR/pathfinder.git -b path
cd GLL4Graph
mkdir results
export JAVA_HOME='/usr/lib/jvm/jdk-15.0.2'
mvn compile
cd ..
cd ..
cd front
cmake ./
make
