#!/bin/bash

name=$1
echo "" > ./src/autodef.h
echo "#define RUN_${name^^} " > ./src/autodef.h

adj_m="./datasets/${name,,}/${name,,}_cites.txt"
if [ ! -f "$adj_m" ];then
    echo "Cannot find the adj matrix for $name!"
    ./genSource.sh -name $name -genadjm True
fi

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
python3 ./src/statistic.py ${name,,}
echo "Finish!"