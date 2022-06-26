#include<iostream>
#include<string>
#include<queue>
#include<vector>
#include<utility>
#include<algorithm>

using namespace std;

//{{hub_list}, {island_node list}}
typedef pair<vector<int>,vector<int>> Island;

/*
//顶点类型
class node
{
    int id=0;  //顶点id
    vector<int> adj_list={}; //邻接列表，存访邻居顶点的index
    vector<int> features={};  //特征向量

public:
    node(int i, vector<int> adj,vector<int> fetr):id{i}, adj_list{adj},features{fetr}{}

    int get_id(){return id;}
    int get_degree(){return adj_list.size();}
    vector<int> get_adjList(){return adj_list;}
    vector<int> get_feature(){return features;}
};*/

struct node
{
    int id=-1;  //vertex id
    bool valid=true;
    vector<int> adj_list={}; //邻接列表，存访邻居顶点的index
    vector<int> features={};  //特征向量

    node(){}; //默认构造函数，用于不初始化实例化结构体
    //自定义构造函数，成员全部初始化
    node(int i, vector<int> a, vector<int> f):id{i},adj_list{a},features{f}{};
    int get_degree(){return adj_list.size();}

    void invalid(){valid=false;};

};


class island_locator
{
private:
    //枢纽结点buffer
    queue<node> hub_buffer={};
    //task
    queue<pair<int,int>> tasks={};
    //nodes that have been visited
    vector<int> Vglobal={};
    //hubs that have been used as central node to build hub_islands
    vector<int> Hlocal={};
    //the least number of nodes in an island
    int Cmin=2;

public:
    //顶点列表
    vector<node> nodeList={};
    //island列表
    vector<Island> Lislands={};

    //类构造函数,无构造参数
    island_locator(){}
    //类构造函数，带初始化参数
    island_locator(vector<node> nL):nodeList{nL}{}

    //发现hub结点
    void detect_hub(int p1, int THtmp);
    //生成BFS遍历任务
    void task_assign();
    //进行BFS遍历并生成islands
    void TP_BFS(int TH,int Cmax,int p2);

    //检查某顶点是否已经被分类为hub_node或者island_node
    bool if_already_classifed(node& destNd);

    //在遇到某顶点已被访问过时，remov vlocal from vglobal
    void remove_vlocal_from_vglobal(vector<int>,vector<int>&);
    
    // 将初步生成的island放入Lislands，并合并hub周围单个岛顶点的small island
    void push_and_merge(Island&,int);

    //为island添加shell
    void add_shell(Island&);

    //构建hub_islands
    void build_hubIslands();

    //print the Lisaldns
    void prtLislands();

};