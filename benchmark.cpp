#include<benchmark/benchmark.h>
#include"ThreadPool.hpp"
//CPU Bound
void f(int x){
  int t=0;
  benchmark::DoNotOptimize(t);
  for(int i=0;i<10000;i++)
    for(int j=0;j<100;j++)
      {
        ++x;
        benchmark::ClobberMemory();
      }
}
//IO Bound
void g(){
  this_thread::sleep_for(3s);
}
void thread_cpu(benchmark::State& state){
  for(auto _:state){
    ThreadPool pool(state.range(0));
    for(int i=0;i<(int)1e3;i++){
      pool.submit(f,i);
    }
  }
  state.SetComplexityN(state.range(0));
}
void thread_io(benchmark::State& state){
  for(auto _:state){
    ThreadPool pool(state.range(0));
    for(int i=0;i<(int)1e3;i++){
      pool.submit(g);
    }
  }
  state.SetComplexityN(state.range(0));
}
BENCHMARK(thread_cpu)->Unit(benchmark::kMillisecond)->DenseRange(1,20,1)->Complexity();
BENCHMARK(thread_io)->Unit(benchmark::kSecond)->DenseRange(1,20,1)->Complexity();
BENCHMARK_MAIN();
