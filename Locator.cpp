#include"Locator.h"

//to check if a node in nodeList has been recognized as a hub or a islandNode
bool island_locator::if_already_classifed(node& destNd){
    bool flag=false;
    for(auto p: Lislands){
        vector<int>& hub_nodes=p.first;
        vector<int>& island_nodes=p.second;
        if(find(hub_nodes.begin(),hub_nodes.end(),destNd.id)!=hub_nodes.end() ||\
           find(island_nodes.begin(),island_nodes.end(),destNd.id)!=island_nodes.end()
        ){
            flag=true;
        }
    }
    return flag;
}

//when encounters a node that has been visited by any other pe , local pe will perform this function to
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
//@output none, modify Lisland in place
void island_locator::merge_small_island(Island& island){
    Island& nearest_island=Lislands.back();
    if(nearest_island.first.front()==island.first.front()){
        for(auto n:island.second)
            nearest_island.second.push_back(n);
    }
}

//print the final Lislands
void island_locator::prtLislands(){
    int count=1;
    for(auto island:Lislands){
        auto hubs=island.first;
        auto iNodes=island.second;

        cout<<"*********************************************"<<endl;
        cout<<"island "<<count++<<": "<<endl;

        cout<<"hub_nodes are: ";
        for(auto h:hubs) cout<<h<<" ";
        cout<<endl;

        cout<<"island_nodes are: ";
        for(auto i:iNodes) cout<<i<<" ";
        cout<<endl;

        cout<<"*********************************************"<<endl;
    }
}


//detect hub_node
//@param p1 is the parallelism of hub_dector
//@param THtmp is the threshold to identify a hub
void island_locator::detect_hub(int p1, int THtmp){
    int end_bound=nodeList.size()/p1;
    for(int b=0;b<end_bound;b++){
        //in parallel do
        for(int p=0;p<p1;p++){
            node n0=nodeList[b*p1+p];
            //pop n0 from nodeList
            if(n0.valid){ // id=-1 indicates that it's an invalid node
                nodeList[b*p1+p].invalid();
                if(!if_already_classifed(n0) && n0.get_degree()>=THtmp)
                {
                    //if n0.degree>= THtmp, then it's recognized as a hub
                    hub_buffer.push(n0);
                    cout<<"a hub is detected:"<<"node id="<<n0.id<<endl;
                }                
            }
            else continue;
        }
    }
}

//generate bfs-task
//no input,no output, modify the task queue in place
void island_locator::task_assign(){
    while(!hub_buffer.empty()){
        node cur_node=hub_buffer.front();
        hub_buffer.pop();
        for(auto neighbor:cur_node.adj_list){
            pair<int,int> task={cur_node.id,neighbor};
            cout<<"a task is generated:"<<"("<<cur_node.id<<","<<neighbor<<")"<<endl;
            tasks.push(task);
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
            vector<int> Vlocal;
            //query track of the number of nodes whose neighbors have been visited totally
            //count track of the number of nodes that have been visited by any of the PEs
            
            int query=0,count=1;

            if(nodeList[cur_task.second].get_degree()<TH){
                Vlocal.push_back(cur_task.second);
                if(find(Vglobal.begin(),Vglobal.end(),cur_task.second)==Vglobal.end())
                    Vglobal.push_back(cur_task.second);
                else continue;

                bool breakFlag=false;
                bool discardFlag=false;
                while (query!=count && !breakFlag &&!discardFlag)
                {
                    int node0_id=Vlocal[query];
                    node node0=nodeList[node0_id];
                    for(auto neighbor_id:node0.adj_list){
                        node n=nodeList[neighbor_id];
                        if(n.get_degree()<TH){
                            if (find(Vlocal.begin(),Vlocal.end(),n.id)!=Vlocal.end())
                                continue;
                            else if (find(Vglobal.begin(),Vglobal.end(),n.id)==Vglobal.end())
                            {
                                count+=1;
                                Vlocal.push_back(n.id);
                                Vglobal.push_back(n.id);
                                if(Vlocal.size()>Cmax)
                                    breakFlag=true; 
                            }
                            else{
                                discardFlag=true;
                                remove_vlocal_from_vglobal(Vlocal,Vglobal);
                                break;
                            }
                        }
                        else{
                            if(find(hlocal.begin(),hlocal.end(),n.id)==hlocal.end())
                            {
                                hlocal.push_back(n.id);   
                            }
                        }
                    }
                    query+=1;
                }
                if(!discardFlag){
                    Island new_island {hlocal,Vlocal};

                    cout<<"An island is built！"<<endl;
                    cout<<"hub_nodes are:";
                    for(auto h:new_island.first){
                        cout<<h<<" ";
                    }
                    cout<<", and island_nodes are:";
                    for(auto v:new_island.second){
                        cout<<v<<" ";
                    }
                    cout<<endl;
                    cout<<"------------------------------"<<endl;
                    
                    if(query>Cmin)  //when this built island is too small, then merge it
                        Lislands.push_back(new_island);
                    else
                        merge_small_island(new_island);                
                }
            }
            else continue;
    }
    //}
}


