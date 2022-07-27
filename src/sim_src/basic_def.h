/*
 Created by Dengke Han, on 2022/07/26

 Refer to unigcn_acc by YMY / Colleen
*/

#include<assert.h>
#include<inttypes.h>


//data type, use union to only allow one type of data stored
struct data_unit_t
{
    union{
        uint64_t fix;
        double flt;
    };
};

//hardware module types 
enum module_type_t{
    PE_MODULE_TYPE=0,
    MEM_MODULE_TYPE,
};

/*--------------------------------------------------*/
//to print runtime info for debug
#define debug_print(fmt, x...) \
do \
{ \
    printf("%s[%s,%d,%llu]" fmt, m_object_name.c_str(), __FUNCTION__, __LINE__, get_global_time(), ##x); \
}while(0)

#define error_print(fmt, x...) \
do \
{ \
    printf("%s[%s,%d,%llu]Error: " fmt, m_object_name.c_str(), __FUNCTION__, __LINE__, get_global_time(),  ##x); \
}while(0)\
;assert(0);
/*--------------------------------------------------*/

//global clock 
uint64_t get_global_time();
void advance_global_time();