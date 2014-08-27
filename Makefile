
SRC = $(wildcard *.cpp)
HDR = $(wildcard *.hpp)
OBJ = $(SRC:.cpp=.o)

EXE = overhead

CPPFLAGS =

CXX = g++
CXXFLAGS = -g

LD = $(CXX)
LDFLAGS =
LIBS = -lrt



.PHONY: all clean tests

all: $(EXE) tests

tests:
	$(MAKE) -C tests

clean:
	@rm -fv $(EXE) $(OBJ)
	$(MAKE) -C tests clean

$(EXE): $(OBJ)
	$(LD) -o $(EXE) $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.cpp $(HDR) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

