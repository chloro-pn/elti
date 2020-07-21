CXX := g++
INCLUDE_PATH := -I ../include -I ../third_party/varint
CXXFLAGS := -Wall -std=c++11 -g

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
DEPS := $(SRCS:.cpp=.d)

TARGET := elti_test

$(TARGET) : $(OBJS)
	@echo "making elti_test...";\
	$(CXX) -o $@ $^ -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

%.o : %.cpp
	$(CXX) -o $@ -c $< -MMD -MP $(INCLUDE_PATH) $(CXXFLAGS)

-include $(DEPS)

.PHONY : clean
clean : 
	@rm -rf $(OBJS) $(DEPS) $(TARGET)
