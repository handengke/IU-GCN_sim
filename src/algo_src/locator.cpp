/*
Created by Dengke Han, on 2022/06/10
*/

#include"locator.h"
#include<cmath>

//to decline the threshold for identifying hub_nodes for this round
//@param Thtmp is the threshold of last round for identifying hub_nodes
//@output is the updated threshold of this round
int island_locator::Decay(int THtmp){
    // return (THtmp>2)?std::floor(THtmp*0.8):2;
    return std::floor(THtmp*0.8);
    //return 0.8 THtmp_last_round, to update THtmp each round
}


//to remove nodes which have been identified as hubs or island_nodes from nodeList at the end of each round
//no input/output, change nodeList in place
void island_locator::remove_identified_nodes(){
    for(auto p: Lislands){
        vector<int>& hub_nodes=p.hub_list;
        vector<int>& island_nodes=p.island_nodes;
        for(auto h: hub_nodes) nodeList[h].invalid();
        for(auto n: island_nodes) nodeList[n].invalid();
    }
    for(auto hid: hub_list)
        nodeList[hid].invalid();
}

//to count how many nodes have been identified as hub or island_node
int island_locator::valid_count(){
    int count=0;
    for(auto node:nodeList){
        if(node.valid==true) count++;
        else continue;
    }
    return count;
}

//to check if a node in nodeList has been recognized as a hub or a islandNode
bool island_locator::if_already_identified(node& destNd){
    for(auto p: Lislands){
        vector<int>& hub_nodes=p.hub_list;
        vector<int>& island_nodes=p.island_nodes;
        if(find(hub_nodes.begin(),hub_nodes.end(),destNd.id)!=hub_nodes.end() ||\
           find(island_nodes.begin(),island_nodes.end(),destNd.id)!=island_nodes.end() ||\
           hub_list.find(destNd.id)!=hub_list.end()){
            return true;
        }
    }
    return false;
}

//when encounter a node that has been visited by any other pe , local pe will perform this function to
//remove all of the vlocal nodes from vglobal
void island_locator::remove_vlocal_from_vglobal(vector<int> vlocal, vector<int>& vglobal){
    for(auto nid: vlocal){
        if(find(vglobal.begin(),vglobal.end(),nid)!=vglobal.end()){
            vglobal.erase(find(vglobal.begin(),vglobal.end(),nid));
        }
    }
}

//to merge the small islands, especially the islands that only have one island_node except the hub to build a hub_island
//@param island is the input island
//@param nodeNum is the number of island_nodes of this island, represented by qeury(=count)
//@output none, modify Lisland in place
void island_locator::push_and_merge(Island& island,int nodeNum,int cur_cmax){
    if(!Lislands.empty()){
        Island& nearest_island=Lislands.back();
        if(nearest_island.get_island_size()+nodeNum<cur_cmax){
            for(auto n:island.island_nodes)
                nearest_island.island_nodes.push_back(n);
            // for(auto s:island.shell_nodes)
            //     nearest_island.shell_nodes.push_back(s);
            for(auto h:island.hub_list){
                if(find(nearest_island.hub_list.begin(),nearest_island.hub_list.end(),h)==nearest_island.hub_list.end())
                    nearest_island.hub_list.push_back(h);
            }
            cout<<"this island needs to be merged with its previous island since it's small or its previous one is small!"<<endl;                
        }else{
            cout<<"cannot be merged."<<endl;
            Lislands.push_back(island);
        }
        cout<<"------------------------------"<<endl;
    }else{
        //if Lisalnds is empty,push directly
        cout<<"------------------------------"<<endl;
        Lislands.push_back(island);        
    }
}


//build the hub_islands that are used to update hub nodes
// void island_locator::build_and_push_hubIslands(node& hub){
//     Island hub_island({},{hub.id},hub.adj_list);
//     Lislands.push_back(hub_island);
// }

//print the final Lislands
void island_locator::prtLislands(){
    int count=1;
    set<int> h_counted {};
    set<int> n_counted {};
    for(auto island:Lislands){
        auto hubs=island.hub_list;
        auto iNodes=island.island_nodes;
        // auto shells=island.shell_nodes;

        cout<<"*********************************************"<<endl;
        cout<<"island "<<count++<<": "<<endl;

        cout<<"hub_nodes are: ";
        for(auto h:hubs){
            h_counted.insert(h);
            cout<<h<<" ";
        } 
        cout<<endl;

        cout<<"island_nodes are: ";
        for(auto i:iNodes){
            n_counted.insert(i);
            cout<<i<<" ";
        } 
        cout<<endl;

        // cout<<"shell_nodes are: ";
        // for(auto s:shells)
        //     cout<<s<<" ";
        // cout<<endl;

        cout<<"*********************************************"<<endl;
    }
    cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    cout<<"total nodes number:  "<<hub_list.size()+n_counted.size()<<endl;
    cout<<"in-island nodes number:  "<<h_counted.size()+n_counted.size()<<endl;
    cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    cout<<endl;
}

//to output Lislands into Lisland.txt for statistic
void island_locator::saveLislands(){
    string filepath="./build/Lislands.txt";

    ofstream out(filepath);
    streambuf* oldPoint=cout.rdbuf(out.rdbuf());
    //redirect cout
    cout.rdbuf(out.rdbuf());

    for(auto island:Lislands){
        auto hubs=island.hub_list;
        auto iNodes=island.island_nodes;
        for(auto h:hubs)
            cout<<h<<" ";
        for(auto i:iNodes)
            cout<<i<<" ";
    }
    cout.rdbuf(oldPoint);
}

//detect hub_node
//@param p1 is the parallelism of hub_dector
//@param THtmp is the threshold to identify a hub
void island_locator::detect_hub(int p1, int THtmp){
    int end_bound=nodeList.size()/p1;
    for(int b=0;b<end_bound;b++){
        //in parallel do
        for(int p=0;p<p1;p++){
            node& n0=nodeList[b*p1+p];
            if(n0.valid) //that means it's not removed in last round, i.e. not identified as hub or island_node yet
            {
            //pop n0 from nodeList
                if(!if_already_identified(n0) && n0.get_degree()>=THtmp)
                {
                    //if n0.degree>= THtmp, then it's recognized as a hub
                    hub_buffer.push(n0.id);
                    hub_list.insert(n0.id);
                    //corresponding to Q7 in README
                    // build_and_push_hubIslands(n0);
                    cout<<"a hub is detected:"<<"node id="<<n0.id<<endl;
                }                
                else continue;
            }
            else continue;
        }
    }
}

//generate bfs-task
//no input,no output, modify the task queue in place
void island_locator::task_assign(){
    while(!hub_buffer.empty()){
        int hid=hub_buffer.front();
        node cur_node=nodeList[hid];
        hub_buffer.pop();
        for(auto neighbor:cur_node.adj_list){
            if(nodeList[neighbor].valid)
            {
                pair<int,int> task={cur_node.id,neighbor};
                cout<<"a task is generated:"<<"("<<cur_node.id<<","<<neighbor<<")"<<endl;
                tasks.push(task);
            }
            else continue;
        }
    }
}

//bfs engine to build islands
//@param TH is the threshold to identify the hub node
//@param Cmax is the most number of island_nodes that an island can hold
//@param p2 is the parallelism of bfsEngine
void island_locator::TP_BFS(int TH,int Cmax,int p2){
    while (!tasks.empty())
    {
        //for(int p=0;p<p2;p++){
            pair<int,int> cur_task=tasks.front();
            tasks.pop();
            vector<int> hlocal {cur_task.first};
            vector<int> Vlocal {};
            // vector<int> shells {};

            cout<<"processing task is {"<<cur_task.first<<", "<<cur_task.second<<"}"<<endl;

            //corresponding to Q3 in READE.md
            if(find(Vglobal.begin(),Vglobal.end(),cur_task.second)!=Vglobal.end())
            {
                cout<<"this task is dropped because a0(the BFS start point) has been visited yet!"<<endl;
                continue;
            } 

            //query track of the number of nodes whose neighbors have been visited totally
            //count track of the number of nodes that have been visited by any of the PEs
            int query=0,count=1;
            node& a0=nodeList[cur_task.second];
            //if a0 is not a hub,then put it in Vlocal,otherwise, drop this task and forward this 
            //inter-hub connection information to Island Collector
            if(a0.get_degree()<TH){
                Vlocal.push_back(a0.id);

                bool breakFlag=false;  //when |Vlocal|==Cmax
                bool discardFlag=false;  //when a neighbor of node0 is not in Vlocal but in Vglobal,drop this task

                while (query!=count && !breakFlag &&!discardFlag)
                {
                    int node0_id=Vlocal[query];
                    node node0=nodeList[node0_id];

                    //Q8:2
                    // vector<int> possible_shells {};

                    for(auto neighbor_id:node0.adj_list){
                        node n=nodeList[neighbor_id];

                        if(n.valid)
                        {
                            if(n.get_degree()<TH){
                                //corresponding to Q8:2 in README
                                // if(breakFlag || discardFlag)
                                // {
                                //     shells.push_back(neighbor_id);
                                //     continue;
                                // }

                                if (find(Vlocal.begin(),Vlocal.end(),n.id)!=Vlocal.end())
                                    continue;
                                else if (find(Vglobal.begin(),Vglobal.end(),n.id)==Vglobal.end())
                                {
                                    count+=1;
                                    Vlocal.push_back(n.id);
                                    Vglobal.push_back(n.id);
                                    
                                    //Q8:2
                                    // possible_shells.insert(possible_shells.end(),n.adj_list.begin(),n.adj_list.end());

                                    //corresponding to Q4 in README
                                    if(Vlocal.size()==Cmax)
                                    {
                                        breakFlag=true; 
                                        cout<<"Vlocal.size()=Cmax,so break!"<<endl;
                                        //corresponding to Q8:2 in README
                                        // for(auto nn: n.adj_list)
                                        // {
                                        //     if(find(Vlocal.begin(),Vlocal.end(),nn)==Vlocal.end() && find(hlocal.begin(),hlocal.end(),nn)==hlocal.end())
                                        //         shells.push_back(nn);
                                        //     else continue;
                                        // }
                                        break;
                                    } 
                                }
                                else{
                                    discardFlag=true;
                                    
                                    //corresponding to Q8:2 in README
                                    // shells.push_back(n.id);

                                    // for (auto s:possible_shells)
                                    // {
                                    //     if(find(Vlocal.begin(),Vlocal.end(),s)==Vlocal.end() && find(hlocal.begin(),hlocal.end(),s)==hlocal.end())
                                    //         shells.push_back(s);
                                    //     else continue;
                                    // }

                                    //corresponding to Q2 in README.md
                                    remove_vlocal_from_vglobal(Vlocal,Vglobal);
                                    break;
                                }
                            }
                            else{
                                //corresponding to Q1 in READE.md
                                if(find(hlocal.begin(),hlocal.end(),n.id)==hlocal.end())
                                    hlocal.push_back(n.id);   
                            }
                        }
                        else continue;
                    }
                    query+=1;
                }
                
                if (discardFlag){//drop this task and waits for the next
                    cout<<"this task has been dropped since this region of graph have been visited by other PEs!(discardFlag=true)"<<endl;
                    continue;
                }
                else{ 
                    cout<<"this task finished normally!(discardFlag=false)"<<endl;

                    Island new_island(hlocal,Vlocal);
                    
                    //corresponding to Q5 in READEME.md
                    if(Vlocal.size()==1) Vglobal.push_back(Vlocal.front());

                    cout<<"An island is built！"<<endl;
                    cout<<"hub_nodes are:";
                    for(auto h:new_island.hub_list){
                        cout<<h<<" ";
                    }
                    cout<<", island_nodes are:";
                    for(auto v:new_island.island_nodes){
                        cout<<v<<" ";
                    }
                    // cout<<", and shell_nodes are:";
                    // for(auto s:new_island.shell_nodes){
                    //     cout<<s<<" ";
                    // }
                    cout<<endl;
                        
                    //push new island into Lisland, and check if it's too small so that should be merged
                    // or should be merged with previous small island
                    //corresponding to Q7 in README.md
                    //push_and_merge(new_island,query,Cmax);
                    Lislands.push_back(new_island);
                    cout<<"------------------------------"<<endl;
                }
            }
            else 
            { //drop this task
                cout<<"this task has benn dropped since a0(the start point of BFS) is a hub too!"<<endl;
                //send_hub_connection();
                continue;
            }
    }
    //} 
}


