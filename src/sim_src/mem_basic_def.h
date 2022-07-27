/*
 Created by Dengke Han, on 2022/07/19

 Refer to unigcn_acc by YMY / Colleen
*/
#include<autodef.h>
#include<inttypes.h>
#include<basic_def.h>

#define DATA_WIDTH                            (4) //bytes

#define KB_SIZE                              (1024)
#define MB_SIZE                            (1024*1024)
#define GB_SIZE                         (1024*1024*1024)


#ifdef RUN_CORA
#define INPUT_FEATURE_VEC_SIZE               (1433)
#define OUTPUT_FEATURE_VEC_SIZE              (128)
#define GRAPH_VERTEX_AMOUNT                  (2708)
#define GRAPH_EDGE_AMOUNT                    (10556)
#define GRAPH_EPTR_AMOUT                (GRAPH_VERTEX_AMOUNT+1)
#endif


#define PE_FRAME_AMOUNT                        8

//on-chip memory configuation
#define EXT_MEM_ALIGN_SIZE                (64 * MB_SIZE)

//active vertex type
struct actv_vert_t{
    //the feature vector of current active vertex
    data_unit_t prop[INPUT_FEATURE_VEC_SIZE];
};

//final vertex property type
struct vert_prop_t{
    data_unit_t prop[OUTPUT_FEATURE_VEC_SIZE];
};

//edge type
struct edge_t{
    uint64_t srcid;
};

//edge ptr in offset array
struct edge_ptr_t{
    uint64_t eptr;
};

//pe workload type
enum workload_type_t{
    EDGE_WORKLOAD_TYPE,
    VERTEX_WORKLOAD_TYPE
};

//workload message type
struct workload_msg_t{
    workload_type_t type;
    //for edge workload
    data_unit_t value;
    uint64_t edge_ptr;
    int64_t edge_cnt;
    //for vertex workload
    uint64_t ver_pos;
    int64_t ver_cnt;
    //if this workload is the last one
    bool bLast;
};

//to tell which phase is this entry in
enum schedule_phase_t {
    IN_AGG=0,
    IN_COMB
};

//to tell how many operands does this entry need
enum rf_entry_type_t{
    IN_8_OPERAND=0,
    IN_4_OPERAND,
    IN_2_OPERAND
};

//the status of ready-to-fire buffer's entry
struct rf_entry_status_t{
    uint64_t main_vert_id;
    uint64_t neighb_vert_id;

    schedule_phase_t process_phase;
    rf_entry_type_t entry_type;
    bool if_cache_hit;
};



