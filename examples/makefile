CXX := g++
INCLUDE_PATH := -I ../include -I ../third_party/varint
CXXFLAGS := -Wall -std=c++11 -O3

LIBS := -L../src -lelti -L../third_party/varint -lvarint

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.out)
DEPS := $(SRCS:.cpp=.d)

.PHONY :  all
all : $(OBJS)

%.out : %.cpp  ../src/libelti.a
	$(CXX) -o $@ $< -MMD -MP $(INCLUDE_PATH) $(LIBS) $(CXXFLAGS)

../src/libelti.a :
	$(MAKE) -C ../src

-include $(DEPS)

.PHONY : clean
clean : 
	@rm -rf $(OBJS) $(DEPS)
