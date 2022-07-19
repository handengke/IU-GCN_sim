'''
author: Dengke Han
date: 2022/06/18
'''

import numpy as np
import pandas as pd
import sys

args=sys.argv

#the name of selected dataset
dataSet_name=args[1]

#the source path of the dataset
path="./{name}".format(name=dataSet_name)

print(path)

#read origin dataset
data_cites=pd.read_csv('{name}/{name}.cites'.format(name=dataSet_name),sep='\t',header=None)
print(data_cites.shape)

data_content=pd.read_csv('{name}/{name}.content'.format(name=dataSet_name),sep='\t',header=None)
print(data_content.shape)

#for cora
if dataSet_name=='cora':
    # transform all paper ids into the range of [0:2707] (2708 papers included total)
    ct_idx=list(data_content.index)
    paper_id=list(data_content.iloc[:,0])
    mp=dict(zip(paper_id,ct_idx))

    #to build adjcent matrix
    mlen=data_content.shape[0]
    adjm=np.zeros((mlen,mlen))

    # to form an undirected adjm
    for i,j in zip(data_cites[0],data_cites[1]):
        x=mp[i]
        y=mp[j]
        adjm[x][y]=adjm[y][x]=1

    print(adjm)


#for citeseer
elif dataSet_name=='citeseer':
    #transform all paper id to string since not all of paper ids in citeseer are int
    ct_idx=list(data_content.index)
    #print(ct_idx)
    paper_id=list(data_content.iloc[:,0])
    paper_id=[str(i) for i in paper_id]
    mp=dict(zip(paper_id,ct_idx))
    #print(mp['cassell99fully'])

    #to build its adjacent matrix
    mlen=data_content.shape[0]  # the total number of papers
    adjm=np.zeros((mlen,mlen))

    # to form an undirected adjm
    for i,j in zip(data_cites[0],data_cites[1]):
        if str(i) in mp.keys() and str(j) in mp.keys():
            x=mp[str(i)]
            y=mp[str(j)]
            adjm[x][y]=adjm[y][x]=1
    
    print(adjm)

#save the adjm into an txt file so that c++ source code can read and process it 
np.savetxt('./{name}/{name}_cites.txt'.format(name=dataSet_name), np.c_[adjm],fmt='%d',delimiter='\t')