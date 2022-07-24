#!/bin/bash

dataSet_name=$1
echo "Generating adjacent matrix for ${dataSet_name,,}!"
python3 ./src/genAdj.py ${dataSet_name,,}
echo "Generation Complete!"