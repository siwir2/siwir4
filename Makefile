CXX = g++
CXXFLAGS ?= -ansi -Wall -Winline -Wshadow -O3

.PHONY: all clean 

all: raytr
clean:
	rm raytr raytr.o

raytr: raytr.o
	$(CXX) $(CXXFLAGS) -o $@ $^ 

raytr.o: raytr.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
