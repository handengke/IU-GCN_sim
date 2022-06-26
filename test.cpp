#include<iostream>
#include<vector>
#include<algorithm>
#include<omp.h>
#include<thread>
#include<bits/stdc++.h>
#include<sstream>
using namespace std;

void test(int& count){
    cout<<"i'm a thread! My id is "<<this_thread::get_id()<<endl;
    count+=1;
    cout<<"my count is "<<count<<endl;
}

void readcsv(string filename, vector<vector<int>>& edges){
    ifstream inFile(filename);
    string lineStr;
    while(getline(inFile,lineStr))
    {
        //cout<<"--"<<lineStr<<"--"<<endl;
        //将结果保存为二维表结构
        stringstream ss(lineStr);
        string str;
        vector<int> lineArray;
        //devide the numbers by "\t"
        while(getline(ss,str,'\t'))
        {
            istringstream ss(str);
            int temp=0;
            ss>>temp;
            lineArray.push_back(temp);
        }
        edges.push_back(lineArray); 
    }
}

int main(){
    // vector<int> v {1,2,4,5,6};
    // remove(v.begin(),v.end(),4);
    // v.erase(find(v.begin(),v.end(),4));
    // for(auto e: v)
    //     cout<<e<<' ';

    // cout<<endl;
    // cout<<v.size()<<" "<<v.capacity();
    // vector<int> v1{1,3,4,5};
    // cout<<"the last number of v is: "<<v.back()<<endl;
    // if(v.front()==v1.front()) cout<<"YES!!"<<endl;
    int i=5;
    while (i--)
    {
        vector<int> v;
        v.push_back(i);
        cout<<"the size of v is "<<v.size()<<endl;
    }
    
    return 0;
//omp.cc
    // int count=0;
    // thread* th=new thread[4];
    // for(int i=0;i<4;i++){
    //     th[i]=thread(test,ref(count));
    // }
    //  for (int i = 0; i < 4; i++)
    //     th[i].join();//等待子线程结束
    // vector<vector<int>> edgList;
    // string filename="./test_file";
    // readcsv(filename,edgList);

    // for(auto e: edgList)
    // {
    //     cout<<e[0]<<"---"<<e[1]<<endl;
    // }

}