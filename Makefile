CC=g++
std=c++20
flags=-I "benchmark/include" -L "benchmark/build/src" -lbenchmark -lpthread -O3
all: benchmark1 benchmark.o lib libThreadPool.a ThreadPool.o
benchmark1: benchmark.o
	$(CC) benchmark.o libThreadPool.a -o benchmark1
benchmark.o: 
	$(CC) benchmark.cpp -c -o benchmark.o -std=$(std) $(flags)
lib: libThreadPool.a ThreadPool
	
libThreadPool.a: ThreadPool.o
	ar rvs libThreadPool.a ThreadPool.o 
ThreadPool.o:
	$(CC) ThreadPool.cpp -c -o ThreadPool.o -std=$(std) $(flags)
clean:
	rm benchmark1 benchmark.o ThreadPool.o ThreadPool.a 
