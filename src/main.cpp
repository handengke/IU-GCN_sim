/*
Created by Dengke Han, at 2022/06/10
*/

#include<iostream>
#include<vector>
#include<string>
#include<bits/stdc++.h>
#include "Locator.h"
#include "autodef.h"
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
        if(n.valid)
        {
            cout<<"node id: "<<n.id<<endl;
            cout<<"its adjList: ";
            for(auto e:n.adj_list) cout<<e<<" ";
            cout<<endl;
        }
        else continue;
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

#ifdef RUN_TOY
void run_toy(){
    vector<vector<int>> m=\
    {
        {0,1,1,1,0,0,0,0,0,0},
        {1,0,0,1,1,0,0,0,0,0},
        {1,0,0,1,1,0,0,0,0,0},
        {1,1,1,0,1,1,0,1,0,0},
        {0,1,1,1,0,1,0,0,0,0},
        {0,0,0,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,1,0,1},
        {0,0,0,1,0,0,1,0,1,1},
        {0,0,0,0,0,0,0,1,0,1},
        {0,0,0,0,0,0,1,1,1,0}
    };


    vector<node> nList;
    build_nodeList(m,nList);
    //prtList(nList);

    cout<<endl;

    //实例化一个locator
    island_locator locator=island_locator(nList);
    prtList(locator.nodeList);

    int round=1;
    int Thtmp=4;

    while(locator.valid_count()!=0){
        if(round!=1)
            Thtmp=locator.Decay(Thtmp);

        cout<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        cout<<"round "<<round<<",and the Thtmp is "<<Thtmp<<",the valid_count is "<<locator.valid_count()<<endl;
        cout<<endl;

        //p1=3
        locator.detect_hub(3,Thtmp);

        cout<<endl;
        prtList(locator.nodeList);

        locator.task_assign();
        cout<<endl;

        //th,cmax,p2
        locator.TP_BFS(Thtmp,4,2);

        locator.prtLislands();

        locator.remove_identified_nodes();

        cout<<"round "<<round++<<" finish"<<",and the Thtmp is "<<Thtmp<<",the valid_count is "<<locator.valid_count()<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    }
}
#endif

#ifdef RUN_CORA
void run_cora(){
    vector<vector<int>> adj_m;
    string filename="./datasets/cora/cora_cites.txt";
    readFile(filename,adj_m);
    
    vector<node> nList;
    build_nodeList(adj_m,nList);

    //实例化一个locator
    island_locator locator=island_locator(nList);
    prtList(locator.nodeList);
    int round=1;
    int Thtmp=10;

    while(locator.valid_count()!=0){
        if(round!=1)
            Thtmp=locator.Decay(Thtmp);

        cout<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        cout<<"round "<<round++<<",and the Thtmp is "<<Thtmp<<",the valid_count is "<<locator.valid_count()<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        cout<<endl;

        //p1=3
        locator.detect_hub(3,Thtmp);

        cout<<endl;
        prtList(locator.nodeList);

        locator.task_assign();
        cout<<endl;

        //th,cmax,p2
        locator.TP_BFS(Thtmp,200,2);

        locator.prtLislands();
        locator.remove_identified_nodes();
    }
    locator.saveLislands();
}
#endif

#ifdef RUN_CITESEER
void run_citeseer(){
    vector<vector<int>> adj_m;
    string filename="./datasets/citeseer/citeseer_cites.txt";
    readFile(filename,adj_m);
    
    vector<node> nList;
    build_nodeList(adj_m,nList);

    //实例化一个locator
    island_locator locator=island_locator(nList);
    prtList(locator.nodeList);
    int round=1;
    int Thtmp=10;

    while(locator.valid_count()!=0){
        if(round!=1)
            Thtmp=locator.Decay(Thtmp);

        cout<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        cout<<"round "<<round++<<",and the Thtmp is "<<Thtmp<<",the valid_count is "<<locator.valid_count()<<endl;
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
        cout<<endl;

        //p1=3
        locator.detect_hub(3,Thtmp);

        cout<<endl;
        prtList(locator.nodeList);

        locator.task_assign();
        cout<<endl;

        //th,cmax,p2
        locator.TP_BFS(Thtmp,1000,2);

        locator.prtLislands();
        locator.remove_identified_nodes();
    }
}
#endif

int main(){
    #ifdef RUN_TOY
        run_toy();
    #endif

    #ifdef RUN_CORA
        run_cora();
    #endif

    #ifdef RUN_CITESEER
        run_citeseer();
    #endif
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
