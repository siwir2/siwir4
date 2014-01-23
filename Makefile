CXX = g++
CXXFLAGS ?= -ansi  -Winline -Wshadow -O3
SOURCES = raytr.cpp raytracing_serial.cpp PGM.cpp
OBJECTS = raytr.o raytracing_serial.o PGM.o
HEADERS = header.h
LIBS = -lm


${OBJECTS}: ${HEADERS}
 
.SUFFIXES:

#generic compilation rule
%.o : %.cpp
	${CXX} ${CXXFLAGS} -c $<

#how to link
raytr: ${OBJECTS}
	${CXX} -o $@ ${OBJECTS} ${LIBS}

clean:
	rm -f *.o *~ 

