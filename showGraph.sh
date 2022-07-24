#!/bin/bash

name=$1
adj_m="./datasets/${name,,}/${name,,}_cites.txt"
if [ -f "$adj_m" ];then
    python3 ./src/graphVisual.py $name
else
    echo "Cannot find the adj matrix for $name! Please generate it first!"
fi