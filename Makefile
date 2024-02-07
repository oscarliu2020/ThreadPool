CC=g++
std=c++20
flags=-I "benchmark/include" -L "benchmark/build/src" -lbenchmark -lpthread -O3
all: benchmark benchmark.o lib ThreadPool.a ThreadPool.o
benchmark: benchmark.o
	$(CC) benchmark.o libThreadPool.a -o benchmark
benchmark.o: 
	$(CC) benchmark.cpp -c -o benchmark.o -std=$(std) $(flags)
lib: libThreadPool.a ThreadPool
	
libThreadPool.a: ThreadPool.o
	ar rvs libThreadPool.a ThreadPool.o 
ThreadPool.o:
	$(CC) ThreadPool.cpp -c -o ThreadPool.o -std=$(std) $(flags)
clean:
	rm benchmark benchmark.o ThreadPool.o ThreadPool.a 
