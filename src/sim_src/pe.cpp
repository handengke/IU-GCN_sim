/*
 Created by Dengke Han, on 2022/07/26

 Refer to unigcn_acc by YMY / Colleen
*/

#include<pe.h>

//???
event_t* pe::cal_actV_addr(param_t* pParam){
    event_t* pEvent=new event_t;
    pEvent->valid_cycle_time=get_global_time()+pParam->exe_cycle_time;
    return pEvent;
}

//to init pe[mod_idx]
void pe::init(uint64_t in_mod_idx){
    m_object_name="PE"+to_string(in_mod_idx);
    m_mod_type=PE_MODULE_TYPE;
    m_mod_idx=in_mod_idx;
    m_max_func_opy_cnt=PE_FRAME_AMOUNT;
    edge_access_cnt=0;
}

//to process incoming event from robuf
bool pe::process_event(event_t* pEvent,vector<event_t*> &fre_event_queue, vector<event_t*> &inf_event_queue){
    //record the type of processing event
    event_type_t event_type=pEvent->event_type;
    //if this event is FUNIT2PE_EVENT_TYPE,i.e. a graph operator fired to this pe for calculation, then process it
    if(event_type==FUNIT2PE_EVENT_TYPE){
        rf_entry_status_t* firing_operation=(rf_entry_status_t*)pEvent->data;
        exe_queue.push_back(firing_operation);
        delete pEvent;

        if(firing_operation->process_phase==IN_AGG){
            debug_print(": receive FUNIT2PE_EVENT_TYPE, agg phase, main_vertex_id=%llu,neighbor_vertex_id=%llu\n",firing_operation->main_vert_id,firing_operation->neighb_vert_id);
        }
        else if (firing_operation->process_phase==IN_COMB){
            debug_print(": receive FUNIT2PE_EVENT_TYPE, comb phase, main_vertex_id=%llu,neighbor_vertex_id=%llu\n",firing_operation->main_vert_id,firing_operation->neighb_vert_id);
        }
    }else{
        error_print("Event should not be processed by PE: %s",event_type);
    }
    return true;
}

//to simulate the operation performed by pe, i.e. allocate exe frame for every graph operator and simulate the caculation
bool pe::advance(vector<event_t*> &fre_event_queue, vector<event_t*> &inf_event_queue){
    if(exe_queue.size()>0){
        int frame_cnt=0;
        for(int i=0;i<exe_queue.size();i++){
            if(frame_cnt==PE_FRAME_AMOUNT) break;
            else{
                rf_entry_status_t* pPE_exe_entry=exe_queue[i];
                //allocate an execution frame for this entry
                frame_cnt++;
                debug_print(": frame %llu execute an operation: main_vertex_id=%llu, neighbour_vertex_id=%llu\n",frame_cnt,pPE_exe_entry->main_vert_id,pPE_exe_entry->neighb_vert_id);
                //remove processed entry from exe queue
                std::vector<rf_entry_status_t*>::iterator pEntry_ptr=exe_queue.begin()+i;
                exe_queue.erase(pEntry_ptr);
                //after removal, should reset the ptr
                i--;
                //when this entry is the last entry of the whole graph, sent finish event to mem
                if((pPE_exe_entry->main_vert_id==GRAPH_VERTEX_AMOUNT-1)&&(pPE_exe_entry->process_phase==IN_COMB))
                    send_finish_event_to_MEM(fre_event_queue);
            }
        }
    }
}

//when the comb phase of the last vertex has been exed, send finish event to MEM to inform it to stop simulation
bool pe::send_finish_event_to_MEM(vector<event_t*>& fre_event_queue){
    //generate finish event, set its properties and push it into the fre_event_queue
    event_t* pEvent=new event_t;
    uint64_t dst_mod_type=MEM_MODULE_TYPE;
    uint64_t dst_mod_idx=0;
    pEvent->valid_cycle_time=get_global_time()+1;
    pEvent->event_type=PE2MEM_FINISH_EVENT_TYPE;
    set_event(false,pEvent,dst_mod_type,dst_mod_idx);
    fre_event_queue.push_back(pEvent);
    return true;
}