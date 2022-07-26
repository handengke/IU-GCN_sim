/*
 Created by Dengke Han, on 2022/07/19

 Refer to unigcn_acc by YMY / Colleen
*/
#include<autodef.h>
#include<inttypes.h>

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


//on-chip memory configuation
#define EXT_MEM_ALIGN_SIZE                (64 * MB_SIZE)

//data type, use union to only allow one type of data stored
struct data_unit_t
{
    union{
        uint64_t fix;
        double flt;
    };
};

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


