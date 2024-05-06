CC=g++
std=c++20
flags=  -O0
inc= -I "./benchmark/include/"
lib= -L "./benchmark/build/src/" -lbenchmark  -pthread 
all: bench 
bench: bench.o
	$(CC) bench.o  -o bench -std=$(std) $(flags) $(lib)
bench.o: bench.cpp
	$(CC) -c bench.cpp -std=$(std) $(flags) $(inc) -o bench.o -flto
.PHONY: clean
clean:
	rm bench bench.o 
