# This is a *Simple Simulator* of IGCN:Island Locator(by Dengke Han)

## Some considerations about the raw algorithm:
- *Q1*: When processing a task {hub0,a0} in which a0 is one of hub0's negihbors, hub0 is also in the adjacent list of a0, and it'll be recognized as a hub, according to the raw algorithm, it'll be added to Hlocal, which is inappropriate here.
  In fact, we should add a `if(a0.neighbor is a hub and not in Hlocal)` logic here to avoid redundant hub0 in Hlocal.
  > This is true.
- *Q2*: Consider n0 is one of a0's nebighbors, if n0 is not in Vlocal but in Vglobal, according to the raw algorithm, first it deletes Vlocal from Vglobal, and then it drops the BFS task but add the {Hlocal, Vlocal} to Lislands.
  In fact, in a parallel hardware platform, this condition indicates that this region which includes n0 **is processing or have been processed** by another engine, and it should not touch it again. However, the nodes that have already visited by this engine and recorded in Vlocal and Vglobal are not been visited by other engines for certain, so why should we remove Vlocal from Vglobal? I think the only thing we should do is to add {Hlocal, Vlocal} to Lislands without remove options here. Or, from another perspective, if we remove Vlocal from Vglobal, then we should not add {Hlocal, Vlocal} to Lislands because of the flowing two main reasons:
  - We remove Vlocal from Vglobal to indicate that this region will be visited by another PE.
  - If we do them both, then we'll find that there exists some nodes which are in Lislands but not in Vglobal, it's obviously not right.  

We decided to remove here, and not add. The raw algorithm in paper does not give a check here so leads to a weird situation that there exists some nodes which have been identified as island_nodes of an island but not in Vglobal for current round.
> This is true.
- *Q3*: The raw algorithm use hub's neighbor as the start point of BFS, however, it only checks if the neighbor's neighbor is in Vglobal, but there is no operation performed here to check if the start point itself has been visited, obviously, if yes, then this task will be dropped, undoubtly. So we add the flowing codes:
    ```c++
    if(find(Vglobal.begin(),Vglobal.end(),cur_task.second)!=Vglobal.end()) continue;
    ```
    If we find the start point in Vglobal, that means it has been identified as an island_node, we drop this task directly and wait for the next one.
> This is true.
- *Q4*: In raw algorithm line 17, if |Vlocal|>Cmax after appending current node into Vlocal and Vglobal, then break. It seems that judgement condition here doesn't conform the definition of Cmax, i.e. the max number of node in an island. I believe '>' should be replaced by '==' to satisfy the origin definiton of Cmax.
> This is true.
- *Q5*: We noticed that when the start point of a task (a neighbor of a hub) has no other neighbors than its hub, then it will form an island by itself but not added into Vglobal. Thus, when another PE encounters this node, it maybe built a small island again. So we add
```C++
if(Vlocal.size()==1) Vglobal.push_back(Vlocal.front());
```
  before it's treated like an normal size island.
> This is true.
- *Q6*: We noticed that when a task{hub,start_point_neighbor} is fired, if the start point has been visited or it's a hub itself, and for a hub with every one of its tasks faces this situation, then this hub will not be attached to any one of the islands,it will form a isolated island itself.
> This is true, but still in <font color="red">TODO</font>.

- *Q7*: We noticed that it'll generate lots of small islands in which there only exists one or two island_nodes except the hubs. This may lead to that the enhancement of the spatial locality will be limited, in the meanwhile, it'll increasing complexity of island management. So we add an `push_and_merge()` func to add islands into Lislands and check if it needs to be merged with other islands according to the minimum number of an island,i.e.Cmin.

    > This is ture. The merge stratage we adpot here is to check if current processing island's size(do not include hubs) plus the previous one's exceeds Cmax, if not , merge them together to form a big island.

- *Q8*：Apparently, organizing vertex in the form of islands can definately improve the spatial locality of island_nodes, however, there still exist two points worth thinking:
    - ~~Which nodes are responsible for updating hubs?~~
    - What if a node in an island has edegs with other nodes in other different island?
    To tackle these issues, （1)we first propose a novel concept,i.e. the hub_island to update the hubs. (2)Additionally we add a shell for every island. The terminology ‘shell’ we use here is corresponding to the second concern we put forward abobe. <u>If an island was built with some edges 'disconnected' because of the limitation from Cmax,or because some graph regions have benn visited by other PEs yet(corresponding to *breakFlag=true* and *discardFlag=true*, respectively), then we record the nodes which are supposed to be one of the island_nodes of this island but actually in other islands as shell_nodes for this island</u>. In this way, in every GCN layer, each island has only communication with the nodes in the same island.
    
    ~~**However, this will raise another thorny issue, i.e. how to synchronize these shell_nodes when they are updated by the islands they originally belong to.**(TODO)~~
> <font color="red">This is still in TODO.</font>

- *Q9*：Since we cannot predict how many "small islands" will be merged to form a big island, what if the number of nodes in the big island becomes larger than the Cmax？
> We can modify the `push_and_merge()` func, i.e. When we prepare to merge an island with others out of its small size, we first consider whether this merge opration will cause the resulted merged island too large, that menas its size exceeds Cmax.

- *Q10*：Consider that some island_nodes in an island are regareded as shell_nodes of a small island, and they must be merged, then after the merge operation, there will be some nodes both in the island_nodes and shell_nodes. How to eliminate this kind of redundancy？
> <font color="red">This is still in TODO.</font>

- *Q11*：We found that after the islands build process has finished, the total number of hubs plus island_nodes is smaller than the origin node numbers. Through data analysis, we found that there exists another type of nodes, it has only one neighbor and so as its neighbor. I think it's unavoidably so I do nothing about this.
> This is relied on the specific dataset we use, and cannot be avoided.

## Workflow
It's easy to start the algorithm simulation:
```Shell
./run.sh CORA
```
The results got will be saved into ./build/run_log.txt, including the detail info about every generated island and some statistical info.
Please note that all paths used in source codes are the path relative to your current path,i.e. the path where run.sh is in. If you wanna excute single source(eg, genAdj.py),please excute them in the top directory:
```Shell
./src/genAdj.py {dataset_name}
```
