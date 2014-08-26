
MEMSTAT_SRC = $(wildcard *.cpp)
MEMSTAT_HDR = $(wildcard *.hpp) $(wildcard *.h)
MEMSTAT_OBJ = $(MEMSTAT_SRC:.cpp=.o)

TARGETS = memstat foo

CPPFLAGS =

CC = tau_cc.sh
CFLAGS = -g

CXX = g++
CXXFLAGS = -g

LD = $(CXX)
LDFLAGS =
LIBS = -lrt



.PHONY: all clean

all: $(TARGETS)

clean:
	@rm -fv $(TARGETS) *.o

memstat: $(MEMSTAT_OBJ)
	$(LD) -o memstat $(MEMSTAT_OBJ) $(LDFLAGS) $(LIBS)

foo: foo.c
	$(CC) foo.c -o foo

%.o: %.cpp $(MEMSTAT_HDR) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

