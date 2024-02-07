CC=g++
std=c++20
flags=-I "./" -lbenchmark -lpthread -O3 -L"./"
all: bench bench.o
bench: bench.o
	$(CC) bench.o -o bench -std=$(std) $(flags)
bench.o: 
	$(CC) bench.cpp -c -o bench.o -std=$(std) $(flags)
.PHONY: clean
clean:
	rm bench bench.o 
