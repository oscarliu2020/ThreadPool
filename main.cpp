#include"ThreadPool.hpp"
#include<bits/stdc++.h>
using namespace std;
using namespace literals::chrono_literals;
mutex io;
uint64_t rnddata[10000];
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dist(1,1000);
int counter=0;
void f(int l,int r){
    sort(rnddata+l,rnddata+r);
}
bool is_asc(int l,int r){
    for(int i=l;i<r-1;i++){
        if(rnddata[i]>rnddata[i+1])return false;
    }
    return true;
}
int main(){
    for(int i=0;i<10000;i++){
        rnddata[i]=dist(gen);
    }
    constexpr int step=2500;
    {
        ThreadPool p(4);
        for(int i=0;i<4;i++){
            p.submit(f,i*step,(i+1)*step);
        }
    }
    int flag=1;
    for(int i=0;i<4;i++){
        flag=(flag&&is_asc(i*step,(i+1)*step));
    }
    cout<<boolalpha<<flag<<endl;;
    return 0;
}