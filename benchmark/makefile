CXX := g++
INCLUDE_PATH := -I ../include -I ../third_party/varint
CXXFLAGS := -Wall -std=c++11

LIBS := -L../src -lelti -L../third_party/varint -lvarint

### jemalloc ###
INCLUDE_PATH += /home/pn/桌面/jemalloc/include
LIBS += -L/home/pn/桌面/jemalloc/lib -ljemalloc

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS.cpp=.out)
DEPS := $(SRCS:.cpp=.d)

.PHONY :  all
all : $(OBJS) ../src/libelti.a
	$(CXX) -o $@ $< -MMD -MP $(INCLUDE_PATH) $(LIBS) $(CXXFLAGS)

../src/libelti.a :
	$(MAKE) -C ../src

-include $(DEPS)

.PHONY : clean
clean : 
	@rm -rf $(OBJS) $(DEPS)
