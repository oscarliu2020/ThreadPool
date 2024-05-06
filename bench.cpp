#include<benchmark/benchmark.h>
#include"ThreadPool.hpp"
#include<chrono>
//CPU Bound
void f(int x){
  int t=0;
  benchmark::DoNotOptimize(t);
  std::chrono::time_point<std::chrono::system_clock> start=std::chrono::system_clock::now();
  while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-start).count()<1000);
}
//IO Bound
void g(){
  this_thread::sleep_for(1s);
}
void thread_cpu(benchmark::State& state){
  for(auto _:state){
    ThreadPool pool(state.range(0));
    for(int i=0;i<(int)1e2;i++){
      pool.submit(f,i);
    }
  }
  state.SetComplexityN(state.range(0));
}
void thread_io(benchmark::State& state){
  for(auto _:state){
    ThreadPool pool(state.range(0));
    for(int i=0;i<(int)1e2;i++){
      pool.submit(g);
    }
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(thread_cpu)->Unit(benchmark::kMillisecond)->DenseRange(1,20,1)->Complexity();
BENCHMARK(thread_io)->Unit(benchmark::kSecond)->DenseRange(1,20,1)->Complexity();
BENCHMARK_MAIN();
