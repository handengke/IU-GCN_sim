#include<iostream>
#include<vector>
#include<string>
#include"Locator.h"

using namespace std;

//构造初始顶点列表
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


//输出列表
void prtList(vector<node>& l){
    for(auto n:l){
        cout<<"node id: "<<n.id<<endl;
        cout<<"its adjList: ";
        for(auto e:n.adj_list) cout<<e<<" ";
        cout<<endl;
    }
    cout<<endl;
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

    vector<node> nList;

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

    locator.TP_BFS(4,5,2);

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