/*
 Created by Dengke Han, on 2022/07/26

 Refer to unigcn_acc by YMY / Colleen
*/
#include<vector>
#include<inttypes.h>
#include<string>
#include "basic_def.h"
using namespace std;

//event type identification
enum event_type_t{
    NONE_VALID_EVENT_TYPE=0,
    FUNIT2PE_EVENT_TYPE,
    PE2MEM_FINISH_EVENT_TYPE,
};

//event type
class event_t{
public:
    //if this event is self-triggered
    bool self_trigger;
    //when this event is valid
    uint64_t valid_cycle_time;
    //the type this object belongs to
    event_type_t event_type;
    
    uint64_t src_mod_type;
    uint64_t src_mod_idx;
    uint64_t dst_mod_type;
    uint64_t dst_mod_idx;

    void* data;

    event_t(){
        self_trigger=false;
        valid_cycle_time=get_global_time();
        src_mod_type=0;
        src_mod_idx=0;
        dst_mod_type=0;
        dst_mod_idx=0;
    };
};

//???
struct param_t{
    uint64_t exe_cycle_time;

    vector<data_unit_t> output;
    vector<data_unit_t> input;
};

//all modules extends this base class
class object
{
public:
    string m_object_name;
    uint64_t m_stage_cnt;
    int64_t m_inf_event_cnt;
    int64_t m_max_inf_event_cnt;
    int64_t m_func_opy_cnt;
    int64_t m_max_func_opy_cnt;

    vector<bool> m_in_fires;

    uint64_t m_mod_type;
    uint64_t m_mod_idx;

    object(){
        m_func_opy_cnt=0;
        m_inf_event_cnt=0;
    };
    
    /*--------------------------------------------------*/
    //every different type of module can define its own func    
    virtual void init(uint64_t mod_idx);

    virtual bool begin(){
        return true;
    };

    virtual bool end(){
        return true;
    }

    virtual bool trigger(vector<event_t *> &event_queue){
        return true;
    }

    virtual bool process_event(event_t* pEvent,vector<event_t*> &fre_event_queue, vector<event_t*> &inf_event_queue){
        return true;
    }

    virtual bool advance(vector<event_t*>& fre_event_queue, vector<event_t*>& inf_event_queue){
        return true;
    };
    /*--------------------------------------------------*/

    //to deline m_inf_event_cnt by 1
    virtual bool clear_event();
    //to block an event
    bool block_event(event_t* pEvent);
    //to release 1 occupancy of this mod
    bool release_occupy();
    //to occupy this mod one more time
    bool func_occupied();
    //to set the properties of this event
    void set_event(bool self_trigger,event_t* pEvent,uint64_t dst_mod_type=0, uint64_t dst_mod_idx=0);

};