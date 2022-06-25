#include<iostream>
#include<vector>
#include<string>
#include<bits/stdc++.h>
#include"Locator.h"

using namespace std;

//build the initial nodeList
void build_nodeList(vector<vector<int>> adjm,vector<node>& ndList)
{
    for(int i=0;i<adjm.size();i++)
    {
        node newNd=node();
        newNd.id=i;
        for(int j=0;j<adjm[i].size();j++){
            if(adjm[i][j]) newNd.adj_list.push_back(j);
        }
        ndList.push_back(newNd);
    }
}

//print the node_list
void prtList(vector<node>& l){
    for(auto n:l){
        cout<<"node id: "<<n.id<<endl;
        cout<<"its adjList: ";
        for(auto e:n.adj_list) cout<<e<<" ";
        cout<<endl;
    }
    cout<<endl;
}

//read the adjacent matrix generated from py
void readFile(string filename, vector<vector<int>>& matrix){
    ifstream inFile(filename);
    string lineStr;
    while(getline(inFile,lineStr))
    {
        //cout<<"--"<<lineStr<<"--"<<endl;
        //将结果保存为二维表结构
        stringstream ss(lineStr);
        string str;
        vector<int> lineArray;
        //devide the numbers by "\t"
        while(getline(ss,str,'\t'))
        {
            istringstream ss(str);
            int temp=0;
            ss>>temp;
            lineArray.push_back(temp);
        }
        matrix.push_back(lineArray); 
    }
}

int main(){
    
    vector<vector<int>> m=\
    {
        {0,0,1,1,0,0,0,0,0},
        {0,0,1,1,0,0,0,0,0},
        {1,1,0,1,1,0,1,0,0},
        {1,1,1,0,0,0,0,0,0},
        {0,0,1,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,1},
        {0,0,1,0,0,1,0,1,1},
        {0,0,0,0,0,0,1,0,1},
        {0,0,0,0,0,1,1,1,0}
    };

    // vector<vector<int>> adj_m;
    // string filename="./datasets/cora/cora_cites.txt";
    // readFile(filename,adj_m);

    vector<node> nList;
    // build_nodeList(adj_m,nList);
    build_nodeList(m,nList);
    prtList(nList);

    cout<<endl;

    //实例化一个locator
    island_locator locator=island_locator(nList);
    prtList(locator.nodeList);

    //p1=3, THtmp=4
    locator.detect_hub(3,4);

    cout<<endl;
    prtList(locator.nodeList);

    locator.task_assign();
    cout<<endl;

    //th,cmax,p2
    locator.TP_BFS(4,100,2);

    locator.prtLislands();

    // node node0=node();
    // node0.id=0;

    // if(locator.if_already_classifed(node0)){
    //     cout<<"yes"<<endl;
    // }
    // else{
    //     cout<<"no"<<endl;
    // }

    return 0;
}
