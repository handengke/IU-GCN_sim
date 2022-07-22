/*
 Created by Dengke Han, at 2022/07/19
*/

#include<iostream>
#include<string>
#include<queue>
#include<vector>
#include<set>
#include<utility>
#include<algorithm>
#include<map>

using namespace std;

//the conceptual tasks
struct island_task
{
    int hub_num=0,iNode_num=0;
    vector<int> hub_ids {};
    vector<int> iNode_ids {};

    //the generated bitmap of the island
    //assume that the first row and so as the first column of bitmap represents the row ID of nodes
    vector<vector<int>> bitmap;

    //to store the map of the global node id and in-island node id
    map<int,int> IID_MAP;

    int round_id=0;
};

//to collect island_tasks
class island_collector
{
private:
    //Island Evaluation Task FIFO
    queue<island_task> IET_FIFO;

    //inter-hub edge map
    vector<vector<int>> INTHUB_MAP;

public:
    island_collector(){};

    //to push a new island task into the FIFO
    void push_task(island_task&){};

    //to get a task for evaluation 
    island_task get_task(){};

    //to generate hub agg task according to INTHUB_MAP
    void gen_hubAgg_task(){};

    //to remap global node id into in-island node id
    void remap_node_id(island_task&){};

};
