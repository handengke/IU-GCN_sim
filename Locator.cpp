#include"Locator.h"

//to check if a node in nodeList has been recognized as a hub or a islandNode
bool island_locator::if_already_classifed(node& destNd){
    bool flag=false;
    for(auto p: Lislands){
        vector<int>& hub_nodes=p.hub_list;
        vector<int>& island_nodes=p.island_nodes;
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
//@param nodeNum is the number of island_nodes of this island, represented by qeury(=count)
//@output none, modify Lisland in place
void island_locator::push_and_merge(Island& island,int nodeNum){
    if(!Lislands.empty()){
        Island& nearest_island=Lislands.back();
        if(!nearest_island.hub_list.empty()){
            if(nodeNum<=Cmin) // if current island is small
            {
                if(nearest_island.hub_list.front()==island.hub_list.front()){
                    for(auto n:island.island_nodes)
                        nearest_island.island_nodes.push_back(n);
                    for(auto s:island.shell_nodes)
                        nearest_island.shell_nodes.push_back(s);
                    cout<<"this island needs to be merged with its previous island since it's small!"<<endl;
                }
                else{
                    Lislands.push_back(island);
                    cout<<"this island cannot be merged since it's the first small island of this hub!"<<endl;
                }                       
            }
            else //current island is not small,but we still have to check if its previous island is small and need to merge with is
            {
                if(nearest_island.island_nodes.size()<=Cmin){
                    if(nearest_island.hub_list.front()==island.hub_list.front()){
                        for(auto n:island.island_nodes)
                            nearest_island.island_nodes.push_back(n);
                        for(auto s:island.shell_nodes)
                            nearest_island.shell_nodes.push_back(s);
                        cout<<"this island needs to be merged with its previous island since previous island is small!"<<endl;
                    }
                    else{
                        Lislands.push_back(island);
                        cout<<"this island dosen't need to be merged since its previous island is not small!"<<endl;
                    }  
                }
                else Lislands.push_back(island);
            }
            cout<<"------------------------------"<<endl;
        }
        else
        {
            cout<<"the previous island is an hub_island and cannot be merged!"<<endl;
            Lislands.push_back(island); 
        } 
    }
    else
    {
        //if Lisalnds is empty,push directly
        cout<<"------------------------------"<<endl;
        Lislands.push_back(island);        
    }
}


//build the hub_islands that are used to update hub nodes
void island_locator::build_and_push_hubIslands(node& hub){
    Island hub_island({},{hub.id},hub.adj_list);
    Lislands.push_back(hub_island);
}

//print the final Lislands
void island_locator::prtLislands(){
    int hubC=0,nodeC=0;
    int count=1;
    for(auto island:Lislands){
        auto hubs=island.hub_list;
        auto iNodes=island.island_nodes;
        auto shells=island.shell_nodes;

        
        if(hubs.empty()) hubC++;

        cout<<"*********************************************"<<endl;
        cout<<"island "<<count++<<": "<<endl;

        cout<<"hub_nodes are: ";
        for(auto h:hubs)
        {
            cout<<h<<" ";
        } 
        cout<<endl;

        cout<<"island_nodes are: ";
        for(auto i:iNodes)
        {
            cout<<i<<" ";
            if(!hubs.empty())
                nodeC++;
        } 
        cout<<endl;

        cout<<"shell_nodes are: ";
        for(auto s:shells)
            cout<<s<<" ";
        cout<<endl;

        cout<<"*********************************************"<<endl;
    }
    cout<<"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"<<endl;
    cout<<"total nodes number: "<<nodeC+hubC;
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
                    //corresponding to OP1 in README
                    build_and_push_hubIslands(n0);
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
            vector<int> Vlocal {};
            vector<int> shells {};

            cout<<"processing task is {"<<cur_task.first<<", "<<cur_task.second<<"}"<<endl;

            //corresponding to Q3 in READE.md
            if(find(Vglobal.begin(),Vglobal.end(),cur_task.second)!=Vglobal.end())
            {
                cout<<"this task is dropped because the BFS start point has been visited yet!"<<endl;
                continue;
            } 

            //query track of the number of nodes whose neighbors have been visited totally
            //count track of the number of nodes that have been visited by any of the PEs
            int query=0,count=1;

            if(nodeList[cur_task.second].get_degree()<TH){
                Vlocal.push_back(cur_task.second);

                bool breakFlag=false;
                bool discardFlag=false;
                while (query!=count && !breakFlag &&!discardFlag)
                {
                    int node0_id=Vlocal[query];
                    node node0=nodeList[node0_id];

                    //OP2:2
                    vector<int> possible_shells {};

                    for(auto neighbor_id:node0.adj_list){
                        node n=nodeList[neighbor_id];

                        if(n.get_degree()<TH){
                            //corresponding to OP2:2 in README
                            if(breakFlag || discardFlag)
                            {
                                shells.push_back(neighbor_id);
                                continue;
                            }

                            if (find(Vlocal.begin(),Vlocal.end(),n.id)!=Vlocal.end())
                                continue;
                            else if (find(Vglobal.begin(),Vglobal.end(),n.id)==Vglobal.end())
                            {
                                count+=1;
                                Vlocal.push_back(n.id);
                                Vglobal.push_back(n.id);
                                
                                //OP2:2
                                possible_shells.insert(possible_shells.end(),n.adj_list.begin(),n.adj_list.end());

                                //corresponding to Q4 in README
                                if(Vlocal.size()==Cmax)
                                {
                                    breakFlag=true; 
                                    cout<<"Vlocal.size()=Cmax,so break!"<<endl;
                                    //corresponding to OP2:2 in README
                                    for(auto nn: n.adj_list)
                                    {
                                        if(find(Vlocal.begin(),Vlocal.end(),nn)==Vlocal.end() && find(hlocal.begin(),hlocal.end(),nn)==hlocal.end())
                                            shells.push_back(nn);
                                        else continue;
                                    }
                                    //break;
                                    continue;
                                } 
                            }
                            else{
                                discardFlag=true;
                                
                                //corresponding to OP2:2 in README
                                shells.push_back(n.id);

                                for (auto s:possible_shells)
                                {
                                    if(find(Vlocal.begin(),Vlocal.end(),s)==Vlocal.end() && find(hlocal.begin(),hlocal.end(),s)==hlocal.end())
                                        shells.push_back(s);
                                    else continue;
                                }

                                //no need for remove, corresponding to Q2 in README.md
                                //remove_vlocal_from_vglobal(Vlocal,Vglobal);
                                continue;
                            }
                        }
                        else{
                            //corresponding to Q1 in READE.md
                            if(find(hlocal.begin(),hlocal.end(),n.id)==hlocal.end())
                                hlocal.push_back(n.id);   
                        }
                    }
                    query+=1;
                }
                Island new_island(hlocal,Vlocal,shells);

                if (discardFlag)
                    cout<<"this task has been dropped since this region of graph have been visited by other PEs!(discardFlag=true)"<<endl;
                else
                    cout<<"this task finished normally!(discardFlag=false)"<<endl;

                cout<<"An island is built！"<<endl;
                cout<<"hub_nodes are:";
                for(auto h:new_island.hub_list){
                    cout<<h<<" ";
                }
                cout<<", island_nodes are:";
                for(auto v:new_island.island_nodes){
                    cout<<v<<" ";
                }
                cout<<", and shell_nodes are:";
                for(auto s:new_island.shell_nodes){
                    cout<<s<<" ";
                }
                cout<<endl;
                    
                //push new island into Lisland, and check if it's too small so that should be merged
                // or should be merged with previous small island
                //corresponding to OP1 in README.md
                push_and_merge(new_island,query);
                // Lislands.push_back(new_island);
                cout<<"------------------------------"<<endl;
            }
            else 
            {
                cout<<"this task has benn dropped since node0 is a hub too!"<<endl;
                continue;
            }
    }
    //} 
}


