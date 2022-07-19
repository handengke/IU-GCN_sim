#!/bin/bash

echo "" > ./src/autodef.h
echo "#define RUN_$1 " > ./src/autodef.h

echo "Generating adjacent matrix from origin dataset."
#python3 ./genAdj.py
echo "Complete!"

echo "Compiling C++ source code!"
make clean
make
echo "Complete!"

echo "Running!"
./main>run_log.txt
echo "Success!"