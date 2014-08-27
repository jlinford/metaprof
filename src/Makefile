
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



.PHONY: all clean

all: $(EXE)
	@mkdir -p ../bin
	@cp -fv $(EXE) ../bin

clean:
	@rm -fv $(EXE) $(OBJ)

$(EXE): $(OBJ)
	$(LD) -o $(EXE) $(OBJ) $(LDFLAGS) $(LIBS)

%.o: %.cpp $(HDR) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

