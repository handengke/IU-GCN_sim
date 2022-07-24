'''
Created by Dengke Han, at 2022/07/24

This file aims to draw graph from a adj matrix
'''

import networkx as nx
import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import sys
import pandas as pd

args=sys.argv

#the name of selected dataset
dataSet_name=args[1]
path="./{name}".format(name=dataSet_name)

#read adj matrix
data_cites=pd.read_csv('./datasets/{name}/{name}_cites.txt'.format(name=dataSet_name),sep='\t',header=None)

#get adj_m
adj_m=np.array(data_cites,dtype=float)

print(adj_m)
 
G = nx.from_numpy_matrix(adj_m)  
nx.draw(G, with_labels=True,font_size=10,node_size=20,width=0.5)
# plt.savefig("{}.svg".format(dataSet_name),bbox_inches='tight')
plt.show()
