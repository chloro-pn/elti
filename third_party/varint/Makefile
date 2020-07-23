CXX := g++
CXXFLAGS := -std=c++11 -Wall -O3
AR := ar

all: lib

lib: 
	$(CXX) -c varint.cpp -o varint.o $(CXXFLAGS)
	$(AR) rcs libvarint.a varint.o

clean:
	rm *.o

test:
	$(CC) test/main.cpp varint.a -o test/main
	./test/main

.PHONY: test
