
SRC = $(wildcard *.cpp)
HDR = $(wildcard *.hpp)
OBJ = $(SRC:.cpp=.o)

EXE = quantify

CPPFLAGS =

CXX = g++
CXXFLAGS = -g -Wall

LD = $(CXX)
LDFLAGS =
LIBS = -lrt



.PHONY: all clean realclean

all: $(EXE)
	@mkdir -p ../bin
	@cp -fv $(EXE) ../bin

clean:
	@rm -fv $(EXE) $(OBJ)

realclean: clean
	@rm -fv ../bin/$(EXE)

$(EXE): $(OBJ)
	$(LD) -o $(EXE) $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.cpp $(HDR) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

