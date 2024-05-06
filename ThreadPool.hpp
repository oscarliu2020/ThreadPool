#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP
#include <cstdint>
#include <mutex>
#include <thread>
#include <functional>
#include <future>
#include <condition_variable>
#include<queue>
#include <iostream>
using namespace std;

class ThreadPool
{
private:
    vector<thread> workers;
    queue<function<void()>> jobs;
    mutex m;
    condition_variable cv;
    bool stop;

public:
    ThreadPool(int = 4);
    ~ThreadPool();
    template <class F, class... Args>
    future<invoke_result_t<F,Args...>> submit(F &&f, Args &&...args);
    void shutdown();
};

ThreadPool::ThreadPool(int max_workers) : stop(false)
{
    for (int i = 0; i < max_workers; i++)
    {
        thread worker([&]
                      {
        while(1){
            unique_lock<mutex> lk(m);
            cv.wait(lk,[&]{return stop||!jobs.empty();});
            if(stop&&jobs.empty())return;
            auto run=jobs.front();
            jobs.pop();
            lk.unlock();
            run();
        } });
        workers.emplace_back(std::move(worker));
    }
}
template <class F, class... Args>
future<invoke_result_t<F,Args...>> ThreadPool::submit(F &&f, Args &&...args)
{
    using ret_type=invoke_result_t<F,Args...>;
    auto task = make_shared<packaged_task<ret_type()>>(bind(forward<F>(f), forward<Args>(args)...));
    future<ret_type> fut=task->get_future();
    {
        unique_lock<mutex> lk(m);
        if (stop)
        {
            cout << "ThreadPool has stopped.\n";
        }
        jobs.push([task]()
                  { (*task)(); });
    }

    cv.notify_one();
    return fut;
}
void ThreadPool::shutdown()
{
    {
        unique_lock<mutex> lk(m);
        stop = 1;
    }
    cv.notify_all();
    for (thread &worker : workers)
    {
        if (worker.joinable())
            worker.join();
    }
}
ThreadPool::~ThreadPool()
{
    if (!stop)
        shutdown();
}
#endif