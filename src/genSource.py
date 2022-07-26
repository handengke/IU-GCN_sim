'''
 Created by Dengke Han, on 2022/06/18
 
Code for: generate adj matrix / csc storage format from origin dataset.
'''

import numpy as np
import pandas as pd
import argparse as ap
import scipy.sparse as sp

parser=ap.ArgumentParser(description='to gen addjm and its csc format')
parser.add_argument("-name",type=str,default='')
parser.add_argument('-genadjm',type=bool,default=True)
parser.add_argument('-gencsc',type=bool,default=False)
args=parser.parse_args()

#the name of selected dataset
dataSet_name=args.name
genAdjm_flag=args.genadjm
genCSC_flag=args.gencsc

#the source path of the dataset
dpath="./datasets/{name}".format(name=dataSet_name)

print(dpath)

#read origin dataset
data_cites=pd.read_csv('{path}/{name}.cites'.format(path=dpath,name=dataSet_name,),sep='\t',header=None)
print(data_cites.shape)

data_content=pd.read_csv('{path}/{name}.content'.format(path=dpath,name=dataSet_name),sep='\t',header=None)
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

if genAdjm_flag:
    #save the adjm into an txt file so that c++ source code can read and process it 
    np.savetxt('{path}/{name}_cites.txt'.format(path=dpath,name=dataSet_name), adjm,fmt='%d',delimiter='\t')

if genCSC_flag:
    #to store the graph in CSC format
    csc_adjm=sp.csc_matrix(adjm)
    #'indptr' is offset, and 'indices' is edge
    print(csc_adjm.indptr,csc_adjm.indices)
    np.savetxt('{path}/{name}.off'.format(path=dpath,name=dataSet_name),csc_adjm.indptr,fmt='%d',delimiter='\n')
    np.savetxt('{path}/{name}.edge'.format(path=dpath,name=dataSet_name),csc_adjm.indices,fmt='%d',delimiter='\n')