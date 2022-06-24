import numpy as np
import pandas as pd
 
# 导入数据：分隔符为空格
path='./datasets/cora/cora_cites'
raw_data = pd.read_csv(path, sep='\t', header=None)
num = raw_data.shape[0]  # 样本点数2708
 
# 将论文的编号转[0,2707]
a = list(raw_data.index)
b = list(raw_data[0])
c = zip(b, a)
map = dict(c)
 
# 将词向量提取为特征,第二行到倒数第二行
features = raw_data.iloc[:, 1:-1]
# 检查特征：共1433个特征，2708个样本点
print(features.shape)
 
labels = pd.get_dummies(raw_data[1434])
print(labels.head(3))
 
raw_data_cites = pd.read_csv(path, sep='\t', header=None)
 
# 创建一个规模和邻接矩阵一样大小的矩阵
matrix = np.zeros((num, num))
# 创建邻接矩阵
for i, j in zip(raw_data_cites[0], raw_data_cites[1]):
    x = map[i]
    y = map[j]  # 替换论文编号为[0,2707]
    matrix[x][y] = matrix[y][x] = 1  # 有引用关系的样本点之间取1


np.savetxt('./datasets/cora/cora_cites.txt', np.c_[matrix],fmt='%d',delimiter='\t')
