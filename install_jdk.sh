#!/bin/sh
wget https://download.java.net/java/GA/jdk15/779bf45e88a44cbd9ea6621d33e33db1/36/GPL/openjdk-15_linux-x64_bin.tar.gz
mv openjdk-15.0.2_linux-x64_bin.tar.gz /usr/lib/jvm
cd /usr/lib/jvm
sudo tar -xzvf openjdk-15.0.2_linux-x64_bin.tar.gz jdk-15.0.2
sudo update-alternatives –-install "/usr/bin/java" "java" "/usr/lib/jvm/jdk-15.0.2/bin/java" 1
