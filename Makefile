CC=clang++
std=c++20
flags=  -O3 -Wno-unqualified-std-cast-call
inc= -I "./"
lib= -L "./" -lbenchmark  -pthread 
all: bench 
bench: bench.o
	$(CC) bench.o  -o bench -std=$(std) $(flags) $(lib)
bench.o: bench.cpp
	$(CC) -c bench.cpp -std=$(std) $(flags) $(inc) -flto
.PHONY: clean
clean:
	rm bench bench.o 
