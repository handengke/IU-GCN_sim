/*
 Created by Dengke Han, at 2022/07/22
*/

#include<iostream>
#include<string>
#include<queue>
#include<vector>
#include<set>
#include<utility>
#include<algorithm>

#include "Collector.h"

using namespace std;

//to perform pre-aggregation so as to remove redundant computation between share neighbor
class pre_aggregator
{

};

//to partion every computation graph into several graph operators and generator tags for them
class partioner
{

};


//to generate graph operators from built island
class OpGenrator
{
private:
    //to store the incoming island evaluation task
    vector<island_task> task_board;

    //island pre-aggregator
    pre_aggregator preAggregator;

    //computational graph partioner
    partioner cgPartioner;

public:
    OpGenrator(){};

    
};