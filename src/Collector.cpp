/*
 Created by Dengke Han, at 2022/07/19
*/

#include "Collector.h"


//to push an island task into task queue
//@param in_task is the task which is going to be pushed in
void island_collector::push_task(island_task& in_task){
    IET_FIFO.push(in_task);
};

//to get an island task from Island Evaluation Task FIFO
island_task island_collector::get_task(){
    island_task out_task=IET_FIFO.front();
    IET_FIFO.pop();
    return out_task;
}

//to generate hub aggregation task based on the inter-hub map
void island_collector::gen_hubAgg_task(){

}

////to remap global node id into in-island node id
void island_collector::remap_node_id(island_task& cur_task){
    for(int i=0;i<cur_task.bitmap.size();i++){
        cur_task.IID_MAP.insert(pair<int,int>(cur_task.bitmap[0][i],i));
    }
}