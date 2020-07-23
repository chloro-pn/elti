CXX := g++
INCLUDE_PATH := -I ../include -I ../third_party/varint
CXXFLAGS := -Wall -std=c++11 -O3

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

TARGET := libelti.a

$(TARGET) : $(OBJS)
	@echo "making libelti.a...";\
	ar crv $@ $^ 

%.o : %.cpp
	$(CXX) -o $@ -c $< -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

-include $(DEPS)

.PHONY : clean
clean : 
	@rm -rf $(OBJS) $(DEPS)
