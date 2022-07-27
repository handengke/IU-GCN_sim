/*
Created by Dengke Han, on 2022/07/19

 Refer to unigcn_acc by YMY / Colleen
*/

#include "basic_def.h"
#include "locator.h"
#include "collector.h"
#include "mem.h"
#include "object.h"
#include "basic_def.h"
#include "pe.h"

class simulator{
private:
    bool compare(event_t* pEvent1, event_t* pEvent2);

public:
    //the name of the specific module
    string sim_object_name;
    //module list, every type of module owns a list
    vector< vector<object* > > sim_object_list;
    //event queue
    vector<event_t*> sim_fresh_event_queue;
    vector<event_t*> sim_infired_event_queue;

    simulator(){
        object_name="simulator";
        fresh_event_queue.clear();
    }

    //to init the simulator, insert necessory modules
    void init();
    //for every module in object_list, call its advance func to \
    finish its operation in this cycle
    void advance();
    //to insert several modules into object list
    void insert_module(module_type_t mod_type, uint64_t mod_amount);
    //to insert fresh event into fresh event list
    void insert_event(event_t* pEvent);
    //to get an event from event list
    event_t* acquire_event();
    //to block an event for some reason
    void block_event(event_t* pEvent);
    //to get excutable event
    void get_exe_event(vector<event_t*>& exe_event_list);
    //to run the simulator
    void run();
}
