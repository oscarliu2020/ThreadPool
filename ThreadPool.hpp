#include<cstdint>
#include<mutex>
#include<thread>
#include<functional>
#include<future>
#include<condition_variable>
#include<iostream>
#include "queue_wrapper.hpp"
using namespace std;

class ThreadPool{
    private:
    vector<thread> workers;
    Queue< function<void()> > jobs;
    mutex m;
    condition_variable cv;
    bool stop;
    public:
    ThreadPool(int=4);
    ~ThreadPool();
    template<class F,class ...Args>
    void submit(F&& f, Args&& ... args);
    void shutdown();

};
ThreadPool::ThreadPool(int max_workers):stop(false){
    for(int i=0;i<max_workers;i++){
        thread worker([&]{
        while(1){
            unique_lock<mutex> lk(m);
            cv.wait(lk,[&]{return stop||!jobs.empty();});
            if(stop&&jobs.empty())return;
            auto run=jobs.front();
            jobs.pop();
            lk.unlock();
            run();
        }
        });
        workers.emplace_back(move(worker));
    }

}
template<class F,class ...Args>
void ThreadPool::submit(F&& f,Args&&... args){
    auto func=bind(forward<F>(f),forward<Args>(args)...);
    {
        unique_lock<mutex> lk(m);
        if(stop){
            cout<<"ThreadPool has stopped.\n";
        }
        jobs.push([=](){func();});
    }
    
    cv.notify_one();
}
void ThreadPool::shutdown(){
    {
        unique_lock<mutex> lk(m);
        stop=1;
    }
    cv.notify_all();
    for(thread& worker:workers){
        if(worker.joinable())
            worker.join();
    }
}
ThreadPool::~ThreadPool(){
    if(!stop)
        shutdown();
}