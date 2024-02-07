CC=g++
std=c++20
flags=-I "./" -lbenchmark -lpthread -O3 -L"./"
all: bench bench.o lib libThreadPool.a ThreadPool.o
bench: bench.o lib
	$(CC) bench.o libThreadPool.a -o bench -std=$(std) $(flags)
bench.o: 
	$(CC) bench.cpp -c -o bench.o -std=$(std) $(flags)
lib: libThreadPool.a ThreadPool.o
libThreadPool.a: ThreadPool.o
	ar rvs libThreadPool.a ThreadPool.o 
ThreadPool.o:
	$(CC) ThreadPool.cpp -c -o ThreadPool.o -std=$(std) $(flags)
.PHONY: clean
clean:
	rm bench bench.o ThreadPool.o libThreadPool.a 
