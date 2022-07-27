/*
Created by Dengke Han, on 2022/07/19

 Refer to unigcn_acc by YMY / Colleen
*/

#include<simulator.h>

//for event_queue sorting
bool simulator::compare(event_t* pEvent1, event_t* pEvent2){
    return pEvent1->valid_cycle_time<pEvent2->valid_cycle_time;
}

//call insert_module to insert neccessary modules into simulator
void simulator::init(){
    insert_module(PE_MODULE_TYPE,64);
}

//to call advance() func for every module in module list
void simulator::advance(){
    uint64_t mod_type_size=sim_object_list.size();
    for(int i=0;i<mod_type_size;i++){
        uint64_t mod_size=sim_object_list[i].size();
        for(int j=0;j<mod_size;j++)
            sim_object_list[i][j]->advance(sim_fresh_event_queue,sim_infired_event_queue);
    }
}

//to insert module into module list
void simulator::insert_module(uint64_t mod_type, uint64_t mod_amount){
    uint64_t mod_size=sim_object_list[mod_type].size();
    //note that modules should be inserted as the order they defined in basic_def.h, otherwise the info about \
    previous modules will be lost after resizing
    sim_object_list.resize(mod_size+1);
    for(int i=0;i<mod_amount;i++){
        object* pMod=NULL;
        switch (mod_type){
        case PE_MODULE_TYPE:
            pMod=new pe;
            break;
        case MEM_MODULE_TYPE:
            pMod=new mem;
            break;
        default:
            error_print("Dont's exist this module type!");
            break;
        }
        pMod->init(i);
        sim_object_list[mod_type].push_back(pMod);
    }
}

//to insert a new event generated to fre_event_queue
void simulator::insert_event(event_t* pEvent){
    sim_fresh_event_queue.push_back(pEvent);
}

//to acquire an event which should be fired at this cycle \
needs to sort infired_queue before calling this func
event_t* simulator::acquire_event(){
    uint64_t queue_size=sim_infired_event_queue.size();
    uint64_t cur_cycle_time=get_global_time();

    event_t* pEvent=sim_infired_event_queue[0];
    if(pEvent->valid_cycle_time==cur_cycle_time){
        sim_infired_event_queue.erase(sim_infired_event_queue.begin()+i);
        return pEvent;
    }
    else if (pEvent->valid_cycle_time<cur_cycle_time){
        error_print("Event not processed at its valid cycle!");
    }
    //since infired_queue was arranged according to the valid_time, this means \
    the earlist event in queue is later than cur_cycle_time, just stop searching in infired_queue

    queue_size=sim_fresh_event_queue.size();
    for(int i=0;i<queue_size;i++){
        event_t* pEvent=sim_fresh_event_queue[i];
        if(pEvent->valid_cycle_time==cur_cycle_time){
            sim_fresh_event_queue.erase(sim_fresh_event_queue.size()+1);
            return pEvent;
        }
        else if(pEvent->valid_cycle_time<cur_cycle_time){
            error_print("Event not processed at its valid cycle!");
        }
    }

    return NULL;
}

//to block event for one cycle
void simulator::block_event(event_t* pEvent){
    pEvent->valid_cycle_time++;
}

//to get an executable event
void simulator::get_exe_event(vector<event_t*> exe_event_list){
    //sort infired event queue with valid time ascending
    sort(sim_infired_event_queue.begin(),sim_infired_event_queue.end(),compare);
    event_t* pEvent=acquire_event();
    vector<event_t*> block_event_list;
    while (pEvent!=NULL){
        bool self_trigger=pEvent->self_trigger;
        if(!self_trigger){
            uint64_t dst_mod_type=pEvent->dst_mod_type;
            uint64_t dst_mod_idx=pEvent->dst_mod_idx;
            bool func_occupied=sim_object_list[dst_mod_type][dst_mod_idx]->func_occupied();
            //if dst mod[dst idx] is not fully occupied, this event can be add to exe event list, \
            otherwise it will be block and return back to the infired event list
            if(func_occupied){
                exe_event_list.push_back(pEvent);
            }
            else{
                block_event(pEvent);
                block_event_list.push_back(pEvent);
            }
        }
        else{
            exe_event_list.push_back(pEvent);
        }
        pEvent=acquire_event();
    }
    sim_infired_event_queue.insert(sim_infired_event_queue.end(),block_event_list.begin(),block_event_list.end());
}

//to run the simulator
void simulator::run(){
    while (true){
        vector<event_t*> exe_event_list;
        get_exe_event(exe_event_list);
        uint64_t event_size=exe_event_list.size();
        //for every event, release one of its dst_mod's occupancy, and remove one of its src_mod's inf event
        for(int i=0;i<event_size;i++){
            event_t* pEvent=exe_event_list[i];
            bool self_trigger=pEvent->self_trigger;
            uint64_t src_mod_type=pEvent->src_mod_type;
            uint64_t src_mod_idx=pEvent->src_mod_idx;
            uint64_t dst_mod_type=pEvent->dst_mod_type;
            uint64_t dst_mod_idx=pEvent->dst_mod_idx;
            if(!self_trigger){
                sim_object_list[dst_mod_type][dst_mod_idx]->release_occupy();
                sim_object_list[src_mod_type][src_mod_idx]->clear_event();
            }
        }
        //for every event, call its dst_mod's processs_event() func to process this event
        for(int i=0,i<event_size;i++){
            event_t* pEvent=exe_event_list[i];
            bool self_trigger=pEvent->self_trigger;
            uint64_t dst_mod_type=pEvent->dst_mod_type;
            uint64_t dst_mod_idx=pEvent->dst_mod_idx;
            sim_object_list[dst_mod_type][dst_mod_idx]->process_event(pEvent,sim_fresh_event_queue,sim_infired_event_queue);
        }
        //advance every module at the same time
        advance();
        advance_global_time();
    }
}