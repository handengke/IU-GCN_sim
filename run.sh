#!/bin/bash

echo "Generating adjacent matrix from origin dataset."
#python3 ./genAdj.py
echo "Complete!"

echo "Compiling C++ source code!"
make
echo "Complete!"

echo "Running!"
./main>log.txt
echo "Success!"