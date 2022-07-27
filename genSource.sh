#!/bin/bash

dataSet_name=$1
echo "Generating adjacent matrix for ${dataSet_name,,}!"
python3 ./src/tools_src/genSource.py -name ${dataSet_name,,}
echo "Generation Complete!"