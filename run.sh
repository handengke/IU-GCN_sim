#!/bin/bash

echo "" > autodef.h
echo "#define RUN_$1 " > autodef.h

echo "Generating adjacent matrix from origin dataset."
#python3 ./genAdj.py
echo "Complete!"

echo "Compiling C++ source code!"
make clean
make
echo "Complete!"

echo "Running!"
./main>log.txt
echo "Success!"