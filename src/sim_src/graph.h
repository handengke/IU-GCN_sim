/*
 Created by Dengke Han, on 2022/07/25

 Refer to unigcn_acc by YMY / Colleen
*/

#ifndef EVENT_CYCLE_SIM_GRAPH_H
#define EVENT_CYCLE_SIM_GRAPH_H
#endif

#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<inttypes.h>
#include<autodef.h>
#include<mem_basic_def.h>
#include<basic_def.h>
using namespace std;

class graph_t{
public:
    //a matrix of node's feature vector 
    actv_vert_t actv_vert_table[GRAPH_VERTEX_AMOUNT];
    //eptr table
    edge_ptr_t eptr_table[GRAPH_EPTR_AMOUT];
    //edge table
    edge_t edge_table[GRAPH_EDGE_AMOUNT];
    //finla vertex property table
    vector<vert_prop_t> vert_prop_table;


    uint64_t edge_table_baddr;
    uint64_t edge_size;

    uint64_t edge_ptr_table_baddr;
    uint64_t edge_ptr_size;

    uint64_t ver_prop_table_baddr;
    uint64_t ver_prop_size;

    uint64_t actVer_table_baddr;
    uint64_t actVer_size;

    uint64_t ver_tmp_prop_table_baddr;
    uint64_t ver_tmp_prop_size;

    uint64_t ver_const_prop_size;
    uint64_t ver_const_prop_table_baddr;

    string dataset_path;
    string dataset_name;
    string off_fname;
    string edge_fname;
    
    uint64_t vertex_cnt;
    uint64_t edge_cnt;

    graph_t(){
        dataset_path="./dataset/";
#ifdef RUN_CORA
        dataset_name="cora";
        cout<<"Dataset : cora"<<endl;
        off_fname=dataset_path+dataset_name+"/"+dataset_name+".off";
        edge_fname=dataset_path+dataset_name+"/"+dataset_name+".edge";
#endif
    };

    //read edges from source file
    void read_edge();
    //read offset array from source file
    void read_offset();
    //to set all elements of fvs to 1.0
    void transform_table();
    //to devide memory into several different parts and record the base address of every part
    void init_table();
};
