#general
CC = g++
CPPFLAGS = -O2 -Wall -std=c++11 
LDFLAGS= 


# #
# SRCS = $(wildcard *.cpp)
# OBJS = $(SRCS:%.cpp=%.o)
OBJS = logic.o main.o 


#target
.PHONY: main clean
main: $(OBJS) 
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJS):%.o:%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean: 
	@echo "cleaning..."
	@rm -f *.o
