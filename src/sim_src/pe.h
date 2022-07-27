/*
 Created by Dengke Han, on 2022/07/26

 Refer to unigcn_acc by YMY / Colleen
*/

#include "object.h"
#include "mem_basic_def.h"
#include "basic_def.h"

class pe: public object{
public:
    //execution latencies of ???
    vector<uint64_t> latencies;

    vector<workload_msg_t> edge_work_msg;
    vector<workload_msg_t> vert_work_msg;

    //rf entry execution queue
    vector<rf_entry_status_t*> exe_queue;

    //as its name
    uint64_t edge_access_cnt;

    //???
    event_t* cal_actV_addr(param_t* pParam);
    //to init pe[mod_idx]
    void init(uint64_t mod_idx);
    //to process incoming event
    bool process_event(event_t* pEvent,vector<event_t*> &fre_event_queue, vector<event_t*> &inf_event_queue);
    //to allocate execution frame for every graph operator and simulate the calculate operation
    bool advance(vector<event_t*> &fre_event_queue, vector<event_t*> &inf_event_queue);
    //when the comb phase of the last vertex has been exed, send finish event to MEM to inform it to stop simulation
    bool send_finish_event_to_MEM(vector<event_t*> &fre_event_queue);
};