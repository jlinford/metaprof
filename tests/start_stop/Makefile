
SRC = $(wildcard *.c)
HDR = $(wildcard *.h)
OBJ = $(SRC:.c=.o)
TAU_OBJ = $(OBJ:.o=.tau.o)

EXE = start_stop
TAU_EXE = start_stop.tau

CPPFLAGS =

TAU_CC = tau_cc.sh
CC = gcc

CFLAGS = -g

LDFLAGS =
LIBS =



.PHONY: all clean realclean

all: $(EXE) $(TAU_EXE)

clean:
	@rm -fv $(EXE) $(TAU_EXE) $(OBJ) $(TAU_OBJ)

realclean: clean

$(EXE): $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDFLAGS) $(LIBS)

$(TAU_EXE): $(TAU_OBJ)
	$(TAU_CC) -o $(TAU_EXE) $(TAU_OBJ) $(LDFLAGS) $(LIBS)

%.o: %.c $(HDR) Makefile
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.tau.o: %.c $(HDR) Makefile
	$(TAU_CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

