/*
 Created by Dengke Han, on 2022/07/26

 Refer to unigcn_acc by YMY / Colleen
*/

#include<basic_def.h>

uint64_t Global_Cycle_Time=0;

uint64_t get_global_time(){
    return Global_Cycle_Time;
}

void advance_global_time(){
    Global_Cycle_Time++;
}
