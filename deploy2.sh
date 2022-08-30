#!/bin/sh
cd third-party
sudo apt install maven
git clone git@github.com:LednevaAR/GLL4Graph.git -b path
cd GLL4Graph
mkdir results
export JAVA_HOME='/usr/lib/jvm/jdk-15.0.2'
mvn compile
cd ..
cd ..
cd front
cmake ./
make
