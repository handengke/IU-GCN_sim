/*
 Created by Dengke Han, on 2022/07/25

 Refer to unigcn_acc by YMY / Colleen
*/

#include<graph.h>

//read edges from source file
void graph_t::read_edge(){
    ifstream infile(edge_fname);
    string line;
    edge_cnt=0;
    while (getline(infile,line)){
        edge_table[edge_cnt].srcid=std::stol(line);
        edge_cnt++;
    }
    if(edge_cnt!=GRAPH_EDGE_AMOUNT){
        cout<<"Dataset ERROR: the amout of edge is wrong!"<<endl;
        exit(-1);
    }
    infile.close();
}

//read offsets from source file
void graph_t::read_offset(){
    ifstream infile(off_fname);
    string line;
    vertex_cnt=0;
    while (getline(infile,line)){
        eptr_table[vertex_cnt].eptr=std::stol(line);
        vertex_cnt++;
    }
    vertex_cnt--;
    if(vertex_cnt!=GRAPH_VERTEX_AMOUNT){
        cout<<"Dataset ERROR: the amout of vertex is wrong!"<<endl;
        exit(-1);
    }
    infile.close();
}

//to set all elements of fvs to 1.0
void graph_t::transform_table(){
    read_edge();
    read_offset();

    uint64_t edge_cnt_tmp = edge_cnt;
    uint64_t vertex_cnt_tmp = vertex_cnt;
    uint64_t edge_ptr_cnt_tmp = vertex_cnt + 1;
    uint64_t feature_vsize = INPUT_FEATURE_VEC_SIZE;

    vert_prop_table.resize(vertex_cnt);
    uint64_t actv_vert_cnt=vertex_cnt;

    //set all elements of all nodes' feature vectors to 1.0
    for(int i=0;i<actv_vert_cnt;i++){
        for(int j=0;j<feature_vsize;j++){
            actv_vert_table[i].prop[j].flt=1.0;
        }
    }
}

//to devide memory into several different parts and record the base address of every part
void graph_t::init_table(){
    transform_table();

    edge_size=sizeof(edge_t)/sizeof(uint64_t) * DATA_WIDTH;
    edge_ptr_size=sizeof(edge_ptr_t)/sizeof(uint64_t) * DATA_WIDTH;
    actVer_size=sizeof(actv_vert_t)/sizeof(uint64_t) * DATA_WIDTH;
    ver_prop_size=sizeof(vert_prop_t)/sizeof(uint64_t) * DATA_WIDTH;

    edge_ptr_table_baddr=0;
    edge_ptr_table_baddr=((edge_ptr_table_baddr+EXT_MEM_ALIGN_SIZE-1)/EXT_MEM_ALIGN_SIZE)*EXT_MEM_ALIGN_SIZE;

    edge_table_baddr=edge_ptr_table_baddr+(vertex_cnt+1)*edge_ptr_size;
    edge_table_baddr=((edge_table_baddr+EXT_MEM_ALIGN_SIZE-1)/EXT_MEM_ALIGN_SIZE)*EXT_MEM_ALIGN_SIZE;

    actVer_table_baddr=edge_table_baddr+edge_cnt*edge_size;
    actVer_table_baddr=((actVer_table_baddr+EXT_MEM_ALIGN_SIZE-1)/EXT_MEM_ALIGN_SIZE)*EXT_MEM_ALIGN_SIZE;

    ver_prop_table_baddr=actVer_table_baddr+vertex_cnt*actVer_size;
    ver_prop_table_baddr=((ver_prop_table_baddr+EXT_MEM_ALIGN_SIZE-1)/EXT_MEM_ALIGN_SIZE)*EXT_MEM_ALIGN_SIZE;

    assert((int64_t)(ver_prop_table_baddr + vertex_cnt * actVer_size + 3 * EXT_MEM_ALIGN_SIZE) > (int64_t)(4 * GB_SIZE));

    cout<<"*****************************Init graph table completed*************************"<<endl;
    
}