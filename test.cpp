#include<iostream>
#include<vector>
#include<algorithm>
#include<omp.h>
#include<thread>
using namespace std;

void test(int& count){
    cout<<"i'm a thread! My id is "<<this_thread::get_id()<<endl;
    count+=1;
    cout<<"my count is "<<count<<endl;
}

int main(){
    /*vector<int> v {1,2,4,5,6};
    // remove(v.begin(),v.end(),4);
    v.erase(find(v.begin(),v.end(),4));
    for(auto e: v)
        cout<<e<<' ';

    cout<<endl;
    cout<<v.size()<<" "<<v.capacity();
    return 0;*/
//omp.cc
    int count=0;
    thread* th=new thread[4];
    for(int i=0;i<4;i++){
        th[i]=thread(test,ref(count));
    }
     for (int i = 0; i < 4; i++)
        th[i].join();//等待子线程结束
}