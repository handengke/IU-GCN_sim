'''
 Created by Dengke Han, on 2022/07/22
 
 This file aims to analyze the result of islandization.
'''

import sys

args=sys.argv

#get the name of using dataset
dataset_name=args[1]

data = []
with open("./build/Lislands.txt","r") as f:
    for line in f.readlines():
        line = line.strip("\n")
        line = line.split()
        line=[int(i) for i in line]
        data.append(line)

not_appear_list=[]

if dataset_name=="cora":
    for i in range(0,2707):
        flag=False
        for line in data:
            if i in line:
                flag=True
        if not flag:
            not_appear_list.append(i)
    print("nodes not appeared in any islands are:")
    print(not_appear_list,'\n')
    print("total num is {num}".format(num=len(not_appear_list)))