CXX := g++
INCLUDE_PATH := -I ../include -I ../third_party/varint -I ../third_party/catch2
LIBS := -L../src -lelti -L../third_party/varint -lvarint
CXXFLAGS := -Wall -std=c++11 -g

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

TARGET := elti_test

$(TARGET) : $(OBJS) ../src/libelti.a ../third_party/varint/libvarint.a
	@echo "making elti_test...";\
	$(CXX) -o $@ $^ -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS) $(LIBS)

%.o : %.cpp
	$(CXX) -o $@ -c $< -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

../src/libelti.a :
	$(MAKE) -C ../src

../third_party/varint/libvarint.a :
	$(MAKE) -C ../third_party/varint

-include $(DEPS)

.PHONY : clean
clean : 
	@rm -rf $(OBJS) $(DEPS) $(TARGET)
