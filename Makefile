CXX=g++
CXXFLAGS=-ansi -Wall -Winline -Wshadow -O3 -pedantic

LIBS = -lnetpbm

.PHONY: all clean 

all: PGM
clean:
	rm -f  PGM.o PGM out.txt
	
PGM: PGM.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
