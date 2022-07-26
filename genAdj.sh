#!/bin/bash

dataSet_name=$1
echo "Generating adjacent matrix for ${dataSet_name,,}!"
python3 ./src/genSource.py -name ${dataSet_name,,}
echo "Generation Complete!"