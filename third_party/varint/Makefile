CXX := g++
CXXFLAGS := -std=c++11 -Wall -O3
AR := ar

all: libvarint.a

libvarint.a: 
	$(CXX) -c varint.cpp -o varint.o $(CXXFLAGS)
	$(AR) rcs libvarint.a varint.o

clean:
	rm *.o libvarint.a

test: libvarint.a
	$(CC) test/main.cpp libvarint.a -o test/main
	./test/main

.PHONY: test
