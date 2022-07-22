#!/bin/bash

echo "" > ./src/autodef.h
echo "#define RUN_$1 " > ./src/autodef.h

dataSet_name=$1
echo "Generating adjacent matrix for ${dataSet_name,,}!"
python3 ./src/genAdj.py ${dataSet_name,,}
echo "Generation Complete!"

echo "Compiling C++ source code!"
exefile="./build/main"
if [ -f "$exefile" ];then
    make clean
fi
make
echo "Compilation Complete!"

echo "Running!"
islandfile="./build/Lislands.txt"
if [ ! -f "$islandfile" ];then
    touch ./build/Lislands.txt
fi
./build/main>./build/run_log.txt
echo "Run Success!"

echo "Analyzing!"
python3 ./src/statistic.py ${dataSet_name,,}
echo "Finish!"